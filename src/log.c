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
#include"include/firebot.h"

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
		Log(-1, log_warning, "Can't find the path to log. Use default log path instead.");
		path = LOG_FILE_PATH;
	}
	if(access(path, F_OK) == -1) {
		char * dir;
		char * copy;

		copy = malloc(strlen(path));
		strcpy(copy,path);
		dir = dirname(copy);
		if (access(dir, W_OK) == -1) {
			Log(0, log_warning, "Can't log to file, so log to console.");
			log_file = stderr;
			return 0;
		}
	}
	log_file = freopen(path, "a",stderr);
	if (!log_file) Log(-1, log_error, strerror(errno));
	return 0;
}

/*
 * Write log message into log file if exists. 
 * exit with status code if LOG_LEVEL if log_error, 
 * otherwise it don`t take effect.  
 */

// There are three log level: log_error, log_warning and log_info 
int Log(int status, LOG_LEVEL level, const char * str)
{
	const char *level_string;
	char *timestamp_string;
	extern FILE *log_file;

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
	fprintf(stderr, "%s : %s : %s\n", timestamp_string, level_string, str);
	free(timestamp_string);
	if (level == log_error) exit(status);
	return 0;
}
