#ifndef _PROFILE_PARSE_H_
#define _PROFILE_PARSE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../debug.h"

#define	MAX_PATH_LEN		(512)
#define	MAX_FILE_NAME_LEN	(128)

int parse_config_file(char *path_to_config_file);
void print_all_vars();
char *get_config_var(char *var_name);


int profile_init(char *profileName, char *appName);
char *profile_getValue(char *appName, char *moduleName, char *key_Name);
int profile_getALL(void);
int profile_release(void);

// int getProfile(char* profile);

#endif

