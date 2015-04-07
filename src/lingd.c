#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <limits.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>
#include <getopt.h>

#include "libbridge.h"
// munge
#include "conf.h"
#include "crypto.h"
#include "random.h"

#include "lingd.h"

static char *io_prefix;

// concatenate /io path entries into single stack allocated string
#define io_path(...)                                                      \
	({                                                                    \
		int count = sizeof((char* []){__VA_ARGS__}) / sizeof(char *) + 1; \
		char *arg[8] = {io_prefix, __VA_ARGS__};                          \
		int len[8];                                                       \
		int size = 0;                                                     \
		for (int i = 0; i < count; ++i) {                                 \
			len[i] = strlen(arg[i]);                                      \
			size += len[i] + 1;                                           \
		}                                                                 \
		char * path = alloca(size + 1);                                   \
		char * p = path;                                                  \
		for (int i = 0; i < count; ++i) {                                 \
			memcpy(p, arg[i], len[i]);                                    \
			p += len[i];                                                  \
			*p++ = '/';                                                   \
		}                                                                 \
		*(p - 1) = '\0';                                                  \
		path;                                                             \
	})

void io_ensure(char *dir) {
	if (strcmp(dir, "/")) {
		io_ensure(dirname(strdupa(dir)));
		mkdir(dir, S_IRWXU);
	}
}

int io_write(char *path, char *str) {
	io_ensure(dirname(strdupa(path)));

	FILE *f = fopen(path, "w");
	if (f == NULL)
		printf("fopen :(\n");

	int n = strlen(str);
	if (fwrite(str, 1, n, f) != n)
		printf("fwrite :(\n");

	fclose(f);
}

int io_read(char *path, char *ptr, int size) {
	FILE *f = fopen(path, "r");
	if (f == NULL)
		return -1;
	int ret = fread(ptr, 1, size - 1, f);
	if (ret > 0 && feof(f)) {
		ptr[ret] = 0;
		ret = 0;
	}
	else {
		ret = -1;
	}
	fclose(f);
	return ret;
}

int foreach_bridge(const char *name, void *arg) {
	*(char **)arg = strdup(name);
	return -1; // finish foreach
}

int init_bridge() {
	char * def_bridge = NULL;
	br_init();
	br_foreach_bridge(foreach_bridge, &def_bridge);
	br_shutdown();

	if (def_bridge == NULL) {
		printf("cant find default bridge :(\n");
		return -1;
	}

	struct ifaddrs *ifaddr;
	if (getifaddrs(&ifaddr) == -1) {
		printf("getifaddrs :(\n");
		return -1;
	}

	for (struct ifaddrs *i = ifaddr; i != NULL; i = i->ifa_next) {
		if (i->ifa_addr == NULL)
			continue;
		if (i->ifa_addr->sa_family != AF_INET)
			continue;
		if (strcmp(i->ifa_name, def_bridge) == 0) {
			char host[NI_MAXHOST];
			if (getnameinfo(i->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0) {
				printf("getnameinfo :(\n");
				return -1;
			}
			io_write(io_path("bridge", def_bridge, "ipaddr"), host);
			break;
		}
	}
	freeifaddrs(ifaddr);
	free(def_bridge);

	return 0;
}

int gateway_ipaddr(char *ipaddr, int ipaddr_size) {
	struct dirent **namelist;
	int count = scandir(io_path("bridge"), &namelist, NULL, alphasort);
	if (count < 0)
		return -1;

	int ret = -1;
	for (int i = 0; i < count; ++i) {
		if (namelist[i]->d_name[0] != '.') {
			if (io_read(io_path("bridge", namelist[i]->d_name, "ipaddr"), ipaddr, ipaddr_size) >= 0) {
				ret = 0;
				break;
			}
			printf("can't read ipaddr of %s :(\n", namelist[i]->d_name);
		}
	}

	if (ret < 0) {
		printf("can't find any gateway :(\n");
	}

	for (int i = 0; i < count; i++)
		free(namelist[i]);
	free(namelist);

	return ret;
}

int find_free_ipaddr(char * start_ipaddr, char * ipaddr, int ipaddr_size) {
	char * start = strdupa(start_ipaddr);
	char * last_octet = strrchr(start, '.');
	if (last_octet == NULL) {
		printf("cant' find octet in %s\n :(", start);
		return -1;
	}
	last_octet++;
	long int l = strtol(last_octet, NULL, 10);
	if (l == LONG_MIN) {
		printf("invalid octet %s :(\n", last_octet);
		return -1;
	}
	*last_octet = '\0';
	snprintf(ipaddr, ipaddr_size, "%s%d", start, l + 1);
	return 0;
}

int boot_app(char *seed, char *dom_config, char *key_file) {
	char buf1[64];
	char buf2[64];
	if (lingd_secret(key_file, buf1, buf2) < 0) {
		printf("can't gen secrets 2 :(\n");
		return -1;
	}

	FILE *f = fopen(dom_config, "a");
	if (f == NULL) {
		printf("can't open %s :(\n", dom_config);
		return -1;
	}

	char epilog[256];
	int bytes = snprintf(epilog, sizeof(epilog), " -9p %s /io /io -secret %s %s\"", seed, buf1, buf2);
	fwrite(epilog, 1, bytes, f);
	fclose(f);

	char cmd[4096];
	snprintf(cmd, sizeof(cmd), "xl create %s", dom_config);
	if (system(cmd) != 0) {
		printf("Failed to boot %s", dom_config);
		return -1;
	}
	return 0;
}

int boot_host(char *name, char *seed, char *key_file) {
	if (lingd_mount(seed)) {
		printf("lingd_mount :(\n");
		return -1;
	}

	io_prefix = malloc(strlen(LINGD_ROOT) + strlen(name) + 2);
	sprintf(io_prefix, "%s/%s", LINGD_ROOT, name);

	init_bridge();

	char gateway[16];
	if (gateway_ipaddr(gateway, sizeof(gateway)) < 0) {
		printf("gateway_ipaddr :(\n");
		return -1;
	}
	char ipaddr[16];
	if (find_free_ipaddr(gateway, ipaddr, sizeof(ipaddr)) < 0) {
		printf("can't find free ipaddr :(\n");
		return -1;
	}

	char * netmask = "255.255.255.0";

	char buf1[64];
	char buf2[64];
	if (lingd_secret(key_file, buf1, buf2) < 0) {
		printf("can't gen secrets :(\n");
		return -1;
	}

	char *arg0 = "/usr/bin/xl";
	char *arg1 = "create";
	char *arg2 = "/dev/null";
	char *arg3 = "name=\"bootling\"";
	char *arg4 = "kernel=\"/io/bootling.img\"";
	char arg5[1024];
	snprintf(arg5, sizeof(arg5),
		"extra=\"-run bootling start %s -ipaddr %s -netmask %s -gateway %s -9p %s /io /io -secret %s %s -home /bootling -pz /bootling/deps/yamerl/ebin /bootling/ebin \" ",
		name, ipaddr, netmask, gateway, seed, buf1, buf2	
	);
	char *arg6 = "memory=64";
	char *arg7 = "vif=['bridge=xenbr0']";

	pid_t pid = fork();
	if (pid < 0) {
		printf("fork :( \n");
		return -1;
	}
	if (pid > 0) {
		if (execl(arg0, arg0, arg1, "-c", arg2, arg3, arg4, arg5, arg6, arg7, NULL) == -1) {
			perror("execl");
			return -1;
		}
	}
	if (setsid() < 0) {
		printf("setsid :(\n");
		return -1;
	}

	char *ctl_path = io_path("ctl");
	while (1) {
		FILE *f = fopen(ctl_path, "r");
		if (f != NULL) {
			char ctl_buf[4096];
			*ctl_buf = '\0';
			fgets(ctl_buf, sizeof(ctl_buf), f);
			fclose(f);
			if (*ctl_buf) {
				f = fopen(ctl_path, "w");
				fclose(f);
				boot_app(seed, ctl_buf, key_file);
			}
		}
		sleep(1);
	}
	return 0;
}

int generate_key(char *key_file) {
	if (key_file == NULL || *key_file == '\0')
		key_file = LINGD_DEFAULT_KEY;

	if (access(key_file, F_OK) != -1) {
		printf("%s already exist\n", key_file);
		return -1;
	}

	printf("Creating secret key: %s - ", key_file);
	char cmd[1024];
	snprintf(cmd, sizeof(cmd), "dd if=/dev/urandom bs=1 count=1024 > %s 2>/dev/null", key_file);
	if (system(cmd) != 0) {
		printf("failed :(\n");
		return -1;
	}
	printf("ok\n");
	return 0;
}

void help() {
	printf("Usage: lingd [options]\n");
	printf("  -s, --server             seed local " LINGD_ROOT " directory\n");
	printf("  -b, --boot=IP:HOST       connect to server IP and boot apps from remote " LINGD_ROOT "/HOST directory\n");
	printf("  -k, --key=FILE           path to secret key; default key is " LINGD_DEFAULT_KEY "\n");
	printf("  -g, --generate-key=FILE  write new secret key to FILE\n");
	printf("  -h, --help               list options\n");
}

int main(int argc, char * argv[]) {
	conf = create_conf();
	conf->key_name = LINGD_DEFAULT_KEY;
	conf->got_force = 1;
	conf->seed_name = "";

	static const struct option options[] = {
		{ .name = "server", .has_arg = no_argument, .val = 's' },
		{ .name = "boot", .has_arg = required_argument, .val = 'b' },
		{ .name = "key", .has_arg = required_argument, .val = 'k' },
		{ .name = "generate-key", .has_arg = required_argument, .val = 'g' },
		{ .name = "help", .val = 'h' },
		{ 0 }
	};
	int o;

	int server_mode = 0;
	char *seed = NULL;
	char *name = NULL;

	while ((o = getopt_long(argc, argv, "hg:k:sb:", options, NULL)) != -1) {
		switch (o) {
		case 's':
			server_mode = 1;
			break;
		case 'b':
			if (server_mode) {
				printf("Please choose eather server or boot mode, not both.\n");
				return -1;
			}
			seed = strdupa(optarg);
			name = strchr(seed, ':');
			if (name == NULL) {
				printf("Invalid boot string\n");
				return -1;
			}
			*name++ = 0;
			break;
		case 'h':
			help();
			return 0;
		case 'g':
			return generate_key(optarg);
		case 'k':
			if (*optarg != '\0')
				conf->key_name = strdup(optarg);
			break;
		default:
			return -1;
		}
	}

	if (!server_mode && name == NULL) {
		help();
		return -1;
	}

	crypto_init();
	random_init(conf->seed_name);
	create_subkeys(conf);

	if (server_mode) {
		lingd_server();
	}
	else if (name) {
		boot_host(name, seed, conf->key_name);
	}

	return 0;
}
