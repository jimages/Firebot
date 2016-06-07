/*
 * Date:    May.24 2016
 * Author:  Zachary Wang(jimages123@gmail.com)
 * Copyright(c) Under MIT License
 * See the full license in root Directory.
 */

#ifndef _FIREBOT_LOG_H_

#define _FIREBOT_LOG_H_

#define LOG_FILE_PATH "/var/log/firebot.log"
int LogInit(void);

enum log_level_t {log_error, log_warning, log_info};

typedef enum log_level_t LOG_LEVEL;

#endif
