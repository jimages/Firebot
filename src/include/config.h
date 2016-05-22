/*
 * Date:    May.20 2016
 * Author:  Zahcary Wang(jimages123@gmail.com)
 * Copyright(c) 2016    Under MIT License
 * See the full License in the root directory.
 */

/* Config header file*/

/* the path of configure file */
#define CONFIG_FILE_PATH "/etc/firebot.conf"

typedef struct config_pair {
    char *name;
    void *value;
} CONFIG;

typedef struct line_table_item {
    void *value;
    struct line_table_item *next;
} LINE_TABLE_ITEM;

int ConfigInit(void);
