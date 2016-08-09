/*
 * Date:    May.18 2016
 * Author:  Zahcary Wang(jimages123@gmail.com)
 * Copyright(c)     Under MIT License
 * See the full license file in the root directory.
 */

#ifndef _FIREBOT_H_

#define _FIREBOT_H_ 
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 2
#define MAX_BUF 256
#define FIREBOT_VERSION "1.0.0dev CAUTION:This is DEVELOPMENT version."

#include<stdio.h>
#include<stdlib.h>
#include<mysql.h>
#include<string.h>
#include<errno.h>
#include<error.h>
#include<strings.h>
#include<time.h>
#include<curl/curl.h>
#include<unistd.h>
#include<stdbool.h>
#include<libgen.h>

#include"config.h"
#include"log.h"
#include"daemon.h"
#include"shell.h"

#endif //_FIREBOT_H_
