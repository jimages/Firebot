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

extern int fb_err(int code, const char *str, ...);
extern int fb_info(const char *str, ...);
extern int fb_warning(const char *str, ...);


#endif //_FIREBOT_LOG_H_
