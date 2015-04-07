#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <pthread.h>

#include "9p.h"
#include "npfs.h"
#include "list.h"
#include "diod_auth.h"
#include "diod_conf.h"
#include "diod_sock.h"
#include "diod_log.h"
#include "npclient.h"
#include "ops.h"

#include "lingd.h"

static Npsrv * srv;
static struct pollfd *fds = NULL;
static int nfds = 0;

void * server_loop(void * arg) {
	int i;

	while(1) {
		for (i = 0; i < nfds; i++) {
			fds[i].events = POLLIN;
			fds[i].revents = 0;
		}
		if (ppoll (fds, nfds, NULL, NULL) < 0) {
			printf("ppoll\n");
			return 0;
		}
		for (i = 0; i < nfds; i++) {
			if ((fds[i].revents & POLLIN)) {
				diod_sock_accept_one(srv, fds[i].fd);
			}
		}
	}
}

int lingd_server() {
	diod_log_init("");
	diod_conf_init();
	diod_conf_set_auth_required(1);
	diod_conf_set_debuglevel(3);

	diod_conf_add_exports(LINGD_ROOT);
	diod_conf_set_exportopts("sharefd");

	List l = diod_conf_get_listen();
	if (!diod_sock_listen(l, &fds, &nfds)) {
		printf("diod_sock_listen\n");
		return -1;
	}

	srv = np_srv_create(16, SRV_FLAGS_AUTHCONN);
	if (!srv) {
		printf("np_srv_create\n");
		return -1;
	}

	if (diod_init(srv) < 0) {
		printf("diod_init\n");
		return -1;
	}

	pthread_t t;
	if (pthread_create(&t, NULL, server_loop, NULL)) {
		printf("pthread_create\n");
		return -1;
	}

	if (pthread_join(t, NULL)) {
		printf("pthread_join\n");
		return -1;
	}

	diod_fini(srv);
	np_srv_destroy(srv);

	return 0;
}

int lingd_mount(char *seed) {
	printf("Connecting to %s: ", seed);
	int fd = diod_sock_connect_inet(seed, "564", DIOD_SOCK_QUIET);
	if (fd < 0) {
		printf("failed\n");
		return -1;
	}
	printf("ok\n");

	int msize = 8192;
	Npcfsys *fs = npc_start(fd, fd, msize, 0);
	if (!fs) {
		printf("npc_start\n");
		return -1;
	}

	printf("Authenticating: ");
	Npcfid * afid = npc_auth(fs, LINGD_ROOT, 0, diod_auth);
	if (!afid) {
		printf("failed\n");
		return -1;
	}
	printf("ok\n");

	Npcfid * root = npc_attach(fs, afid, LINGD_ROOT, 0);
	if (!root) {
		printf("npc_attach\n");
		return -1;
	}

	npc_clunk(afid);
	npc_clunk(root);

	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		printf("fcntl :(\n");

	struct stat st = {0};
	if (stat(LINGD_ROOT, &st) == -1) {
		printf("%s not found, creating: ", LINGD_ROOT);
		if (mkdir(LINGD_ROOT, 0700) != 0) {
			printf("failed\n");
			return -1;
		}
		printf("ok\n");
	}

	printf("Mounting: ");
	char opts[1024];
	snprintf(opts, sizeof(opts),
		"rfdno=%d,wfdno=%d,uname=root,access=user,version=9p2000.L,trans=fd,msize=%d,aname=%s",
		fd, fd, msize, LINGD_ROOT
	);
	if (mount(seed, LINGD_ROOT, "9p", 0, opts) < 0) {
		perror("");
		return -1;
	}
	printf("ok\n");

	return 0;
}
