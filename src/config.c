/*
 * Date:    May.18 2016
 * Author:  Zahcary Wang(jimages123@gmail.com)
 * Copyright(c) 2016   under  MIT License
 * See the full license in root directory.
 */

/*
 * Initializing configure and parsing configure
 */
#include "include/firebot.h"
#include "include/config.h"

#define CONFIG_BUFF 100

static LINE_TABLE_ITEM *root = NULL;

static void next_line(FILE * const f) {
    /* Because open file in r mode, there is no need to convert windows-style newlinw to unix-style newline. */
    while (getc(f) != '\n')
        continue;
} 

static char next_char(FILE * const f) {
    char c;
    
    do {
    c = getc(f);
    } while (c == '\r' || c == ' ');
    return c;
}

static int line_table_push(CONFIG * const config) {
    extern LINE_TABLE_ITEM *root;
    LINE_TABLE_ITEM *current = root, *new;

    new = (LINE_TABLE_ITEM*) malloc(sizeof(LINE_TABLE_ITEM));
    new->value = config;
    new->next = NULL;
    if (root == NULL) {
        root = new;
    } else {
        while (current->next != NULL) 
            current = current->next;
        current->next = new;
    }
    return 0;
}

/* Create an item of a configure pair and return point. */
static CONFIG* create_config(const char *name, const char *value) {
    CONFIG *config;

    config = (CONFIG*) malloc(sizeof(CONFIG));
    config->name = (char*) malloc(strlen(name) + 1);
    config->value = (char*) malloc(strlen(value) + 1);
    strcpy(config->name, name);
    strcpy(config->value, value);
    return config;
}

int ConfigInit() {
    FILE *config_file = NULL;
    char name[CONFIG_BUFF], value[CONFIG_BUFF], c;
    CONFIG *config;
    
    config_file = fopen(CONFIG_FILE_PATH, "r");
    if (config_file == NULL)
        error(-1, errno, CONFIG_FILE_PATH);
    /* Now we read the configure file and make a line table. */
    while ((c = next_char(config_file)) != EOF) {
        short n = 0;

        /* Leap to next line if encounter a blank line. */
        if (c == '\n') {
            next_line(config_file);
            continue;
        }
        /* Leap to next line when encountering a comment. */
        if (c == '#') {
            next_line(config_file);
            continue;
        }

        /* Get the name of a configure pair. */
       while (c != '=' && n < CONFIG_BUFF - 1 ) {
           name[n++] = c;
           c = next_char(config_file);
       }
       name[n++] = '\0'; 
       if ( n > CONFIG_BUFF - 1)
           error(-1, 0, "Fatal error: the length of configure name is out of limit. Please check your configure file.");

       n = 0;
       /* leap the '=' */
       c = next_char(config_file);

       /* Now get the value. */
       while (c != '\n' && n <= CONFIG_BUFF - 1) {
           value[n++] = c;
           c = next_char(config_file);
       }
       value[n++] = '\0';
       if (n > CONFIG_BUFF - 1) 
           error(-1, 0, "Fatal error: the length of configure name is out of limit. Please check your configure file.");

       config = create_config(name, value);

       /* Push it into line table */
       line_table_push(config);
    }
    fclose(config_file);
    return 0;
}

/* Get a certain confiure and return the string of value */
const char * GetConfig(const char * const name) {
    extern LINE_TABLE_ITEM * root;
    CONFIG * current_config;
    LINE_TABLE_ITEM * now = root;

    while (now != NULL) {
        current_config = now->value;
        if (strcasecmp(current_config->name, name) == 0)
            return current_config->value;
        now = now->next;
    }
    /* If there is no specific configure, return NULL. */
    return NULL;
}
