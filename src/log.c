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

/* Add a timestamp at the beginning of log. */
static char * add_timestamp(const char * log) {
    struct tm now_time_tm;
    time_t now_time_time_t = time(NULL);
    char time_string[TIME_STRING_MAX_LENGTH+1];
    char * log_message;

    gmtime_r(& now_time_time_t, & now_time_tm);
    /* At least 20 bytes for the string of date. */
    strftime(time_string, (size_t)(TIME_STRING_MAX_LENGTH+1), "%F %H:%M:%S ", &now_time_tm);
    log_message = malloc((size_t)(strlen(log) + strlen(time_string) + 1));
    if (log_message == NULL)
        error(-1, errno, "Can't genreate log message. May be out of memory.");
    strcpy(log_message, time_string);
    strcat(log_message, log); 

    return log_message;
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
