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
static char * timestamp() {
    struct tm now_time_tm;
    time_t now_time_time_t = time(NULL);
    char * time_string;

    time_string = malloc((size_t)(TIME_STRING_MAX_LENGTH + 1));

    gmtime_r(& now_time_time_t, & now_time_tm);
    /* At least 21 bytes for the string of date. */
    strftime(time_string, (size_t)(TIME_STRING_MAX_LENGTH+1), "%F %H:%M:%S", &now_time_tm);

    return time_string;
}

int LogInit(void) {
    extern FILE *log_file;
    const char * config_value;

    /* If run in root, save messages in log directory. Otherwise to a file specficed by configure or directly print to console. */
        log_file = freopen(LOG_FILE_PATH, "a", stdout);
        log_file = freopen(LOG_FILE_PATH, "a", stderr);
    if(log_file == NULL && (config_value = GetConfig("LOG_FILE_PATH")) != NULL) {
        log_file = freopen(config_value, "a", stderr);
        log_file = freopen(config_value, "a", stdout);
    } 
    /* there is no need to check error, because log will be sent to consule.*/
    return 0;
}

/* Write log message into log file if exists. exit with status code if LOG_LEVEL if log_error, otherwise it don`t take effect. */ 
/* There are three log level: log_error, log_warning and log_info */
int Log(int status, LOG_LEVEL level, const char * str) {
   const char * level_string;
   char * timestamp_string;
   
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
   printf("%s : %s : %s\n", timestamp_string, level_string, str);
   free(timestamp_string);
   if (level == log_error)
       exit(status);
   return 0;
}
