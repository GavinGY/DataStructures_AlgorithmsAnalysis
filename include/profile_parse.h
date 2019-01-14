/**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
#  > Author  ： Gavin | Zhang GuiYang
#  > Mail    ： gavin.gy.zhang@gmail.com
#  > Date    ： Dec/25/2018
#  > Company ： Foxconn·CNSBG·CPEGBBD·RD
#  > Funciton:  profile parse function
#  > Version :  v1.0 
#  > HowToUse:  -
# *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*/

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

#include "debug.h"
#include "config.h"


int profile_init(char *profileName, char *appName);
char *profile_getValue(char *appName, char *moduleName, char *key_Name);
int profile_getALL(void);
int profile_release(void);


#endif

