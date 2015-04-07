#pragma once

#define LINGD_ROOT "/io"
#define LINGD_DEFAULT_KEY "lingd.key"

int lingd_server();
int lingd_mount(char *seed);
int lingd_secret(char *key_file, char buf1[64], char buf2[64]);

#define lingd_err(...) printf("%s\n", )
