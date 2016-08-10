/*
 * Date:    May.24 2016
 * Author:  Zahcary Wang(jimages123@gmail.com)
 * Copyright(c) 2016 Under MIT License
 * See the full license in root diretory.
 */

/*
 * log imformation and errors
 */
#define TIME_STRING_MAX_LENGTH 20 
#include "include/firebot.h"
#include <stdarg.h>

enum log_level_t {log_error, log_warning, log_info};


static FILE *log_file;

/* return a string which refers timestamp. */
static char *timestamp() {
	struct tm now_time_tm;
	time_t now_time_time_t = time(NULL);
	char *time_string;

	time_string = malloc((size_t)(TIME_STRING_MAX_LENGTH + 1));
	gmtime_r(& now_time_time_t, & now_time_tm);
	// At least 21 bytes for the string of date. 
	strftime(time_string, (size_t)(TIME_STRING_MAX_LENGTH+1), 
		"%F %H:%M:%S", &now_time_tm);
	return time_string;
}

int LogInit(void) {
	extern FILE *log_file;
	const char *path;

	path = GetConfig("LOG_FILE_PATH");
	if (!path) {
		fb_warning("Can't find the path to log. Use default log path instead.");
		path = LOG_FILE_PATH;
	}
	if(access(path, F_OK) == -1) {
		char * dir;
		char * copy;

		copy = malloc(strlen(path));
		strcpy(copy,path);
		dir = dirname(copy);
		if (access(dir, W_OK) == -1) {
			fb_warning("Can't log to file, so log to console.");
			log_file = stderr;
			return 0;
		}
		free(copy);
	}
	log_file = freopen(path, "a",stderr);
	if (!log_file) fb_err(EXIT_FAILURE, strerror(errno));
	return 0;
}

/*
 * Write log message into log file if exists. 
 * exit with status code if LOG_LEVEL if log_error, 
 * otherwise it don`t take effect.  
 */

// There are three log level: log_error, log_warning and log_info 
static int fb_log(enum log_level_t level, const char *str, va_list ap)
{
	const char *level_string;
	char *timestamp_string;
	char s[MAX_BUF];

	switch (level) {
	case log_error:
		level_string = "error";
		break;
	case log_warning:
		level_string = "warning";
		break;
	case log_info:
		level_string = "info";
		break;
	}
	timestamp_string = timestamp();
	snprintf(s, sizeof(s), "%s : %s : %s\n",
		 timestamp_string, level_string, str);
	vfprintf(stderr, s, ap);
	free(timestamp_string);
	return 0;
}

extern int fb_warning(const char *str, ...)
{
	va_list ap;

	va_start(ap, str);
	fb_log(log_warning, str, ap);
	va_end(ap);

	return 0;
}

extern int fb_err(int code, const char *str, ...)
{
	va_list ap;

	va_start(ap, str);
	fb_log(log_error, str, ap);
	va_end(ap);

	/*
	 * If we had error in a thread, We do not use exit,
	 * which is not MT-safe. so we exit the thread but not process.
	 * Otherwise we exit the process.
	 */
	// TODO: smoothly exit in mulit-threads environment.
	exit(code);
	return 0;
}

extern int fb_info(const char *str, ...)
{
	va_list ap;

	va_start(ap, str);
	fb_log(log_info, str, ap);
	va_end(ap);

	return 0;
}
