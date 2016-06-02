/*
 * Date:    May.24 2016
 * Author:  Zahcary Wang(jimages123@gmail.com)
 * Copyright(c) 2016 Under MIT License
 * See the full license in root diretory.
 */

/*
 * log imformation and errors
 */
#include"include/firebot.h"

int LogInit(void) {
    FILE *log_file;
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
