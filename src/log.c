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

static FILE *log_file;

int LogInit(void) {
    extern FILE *log_file;

    log_file = fopen(LOG_FILE_PATH, "a");
    return 0;
}
