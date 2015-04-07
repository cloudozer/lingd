#include "lingd.h"

static Npsrv * srv;
static struct pollfd *fds = NULL;
static int nfds = 0;

void * seed_loop(void * arg) {
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


int lingd_seed() {
	diod_log_init("");
	diod_conf_init();
	diod_conf_set_auth_required(1);
	diod_conf_set_debuglevel(3);

	diod_conf_add_exports("/net");
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
	if (pthread_create(&t, NULL, seed_loop, NULL)) {
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