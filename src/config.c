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

static void next_line(FILE * const f)
{
	/*
	 * Because open file in r mode, there is no need to 
	 * convert windows-style newlinw to unix-style newline. 
	 */
	while (getc(f) != '\n')
		continue;
} 

/**
 * Get the map from opt to name of the opt
 *
 * @params	
 * 	char c	the option in command line.
 * @return	
 * 	const *char 	the full option.
 */
static const char *get_conf(char c) {
	extern const struct flag_map flags_map[];

	size_t maxind = sizeof(flags_map) / sizeof(flags_map[0]) - 1;
	for (size_t i = 0 ; i <= maxind ; i++) {
		if (flags_map[i].flag == c)
			return flags_map[i].conf;
	}
	return NULL;
}

/**
 * Load flags and set configure for these.
 * 	@params
 * 		int argc	argc from main.
 * 		char *argv[]	argv from main.
 * 	@return bool
 * 		return 0 if success, otherwise
 * 		return 1;
 */
static bool opt_init(int argc, char *argv[])
{
	char c;
	extern const char optstring[];
	const char *conf;

	if (argc <= 1) {
			conf = get_conf('h');
			AddConfig(conf, "");

			conf = get_conf('v');
			AddConfig(conf, "");
	}

	while (1) {

		c = getopt(argc, argv, optstring);
		if (c == (char)-1) break;
		switch (c) {
		case 'v':
		case 'h':
			conf = get_conf(c);
			AddConfig(conf, "");
			break;
		case 'c':
		case 'l':
			conf = get_conf(c);
			AddConfig(conf, optarg);
			break;
		case 'd':
			conf = get_conf(c);
			if (optarg == 0)
				optarg = "start";
			else if (valid_daemon_command(optarg) != 0)
				fb_err(EXIT_FAILURE,
				       "Invalid daemon command.");
			AddConfig(conf, optarg);
			break;
		case ':':
			fb_warning("Missing option argument.\n");
			out_help();
			exit(EXIT_FAILURE);
			break;
		default: // for '?'
			if (errno != 0)
				fb_err(EXIT_FAILURE, strerror(errno));
			fprintf(stderr, "Unrecognized option -%c.\n", c);
			out_help();
			fb_err(EXIT_FAILURE, "Unrecognized option.\n");
			break;
		}
	}
	return 0;
}
static char next_char(FILE * const f)
{
	char c;

	do {
	c = getc(f);
	} while (c == '\r' || c == ' ');
	return c;
}

int ConfigInit(int argc, char *argv[])
{
	opt_init(argc, argv);
	FILE *config_file = NULL;
	char name[CONFIG_BUFF], value[CONFIG_BUFF], c;
	const char *path;

	if ((path = GetConfig("CONFIG_FILE_PATH")) == NULL)
			path = CONFIG_FILE_PATH;
	config_file = fopen(path, "r");

	if (config_file == NULL) {
		char s[MAX_BUF];
		snprintf(s, sizeof(s), "%s : %s", path, strerror(errno));
		fb_err(EXIT_FAILURE, s);
	}
	// Now we read the configure file and make a line table.
	while ((c = next_char(config_file)) != EOF) {
		short n = 0;

		// Leap to next line if encounter a blank line.
		if (c == '\n') {
		next_line(config_file);
		continue;
		}
		// Leap to next line when encountering a comment.
		if (c == '#') {
			next_line(config_file);
			continue;
		}

		// Get the name of a configure pair.
		while (c != '=' && n < CONFIG_BUFF - 1 ) {
			name[n++] = c;
		c = next_char(config_file);
		}
		name[n++] = '\0'; 
		if ( n > CONFIG_BUFF - 1)
			fb_err(EXIT_FAILURE, "the length of configure name is out of limit. Please check your configure file.");

		n = 0;
		// leap the '='
		c = next_char(config_file);

		// Now get the value.
		while (c != '\n' && n <= CONFIG_BUFF - 1) {
			value[n++] = c;
			c = next_char(config_file);
		}
		value[n++] = '\0';
		if (n > CONFIG_BUFF - 1) 
			fb_err(EXIT_FAILURE, "the length of configure name is out of limit. Please check your configure file.");

		/*
		 * Cause we may define a config before in shell.c
		 * We first check whether the configure is defined.
		 * if exists, we ignore the configure and log a warning.
		 */

		if (GetConfig(name) == NULL) {
			AddConfig(name,value);
		} else {
			char s[MAX_BUF];
			snprintf(s, sizeof(s),
				 "The configure %s is defined before, so ignore the configure in configure file", name);
			fb_warning(s);
		}
	}
	fclose(config_file);
	return 0;
}

/* Get a certain confiure and return the string of value */
const char * GetConfig(const char * const name) 
{
	extern LINE_TABLE_ITEM * root;
	CONFIG * current_config;
	LINE_TABLE_ITEM * now = root;

	while (now != NULL) {
		current_config = now->value;
		if (strcasecmp(current_config->name, name) == 0)
			return current_config->value;
		now = now->next;
	}
	// If there is no specific configure, return NULL.
	return NULL;
}

/* Add a new item of configure. return 0 if success, otherwise if fail.*/
int AddConfig(const char * const name, const char * value)
{
	CONFIG *config;
	extern LINE_TABLE_ITEM *root;
	LINE_TABLE_ITEM *current = root, *new;

	config = malloc(sizeof(CONFIG));
	config->name = malloc((size_t)strlen(name));
	config->value = malloc((size_t)strlen(value));
	strcpy(config->name, name);
	strcpy(config->value, value);

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

int UpdateConfig(const char * const name, const char * value)
{
	extern LINE_TABLE_ITEM *root;
	LINE_TABLE_ITEM *current = root;
	char *new_value;

	// If name or value is NULL.
	if (!name || !value)
		return -1;

	while (strcasecmp(((CONFIG *)current->value)->name, name) != 0) {
		if (current == NULL) return -1;
	current = current->next;
	}
	new_value = malloc((size_t)strlen(value));
	strcpy(new_value, value);
	free(current->value);
	((CONFIG *)current->value)->value = new_value;
	return 0;
}

/**************************************
 * Load the global configure from
 * master if possible.
 *
 * @return int 
 * 	0 if success, otherwise fail.
 **************************************
 */
int LoadNetworkConfig(void)
{
	// Intialize mysql library.
	mysql_library_init(0, NULL, NULL);

	MYSQL * connector;
	const char * host, * user, * passwd, *db;
	unsigned int port;
	char buf[MAX_BUF];
	MYSQL_RES * res;
	MYSQL_ROW row;

	fb_info("Load configures from database.");
	host = GetConfig("MYSQL_ADDR");
	user = GetConfig("MYSQL_USER");
	passwd = GetConfig("MYSQL_PASSWORD");
	db = GetConfig("MYSQL_DATABASE");
	if ( !(host && user && passwd && db))
		fb_err(EXIT_FAILURE, "Can't load configure of Mysql. Maybe you should check you config file.");
	if (!sscanf(GetConfig("MYSQL_PORT"), "%u", &port))
		fb_err(EXIT_FAILURE, strerror(errno));

	connector = mysql_init(NULL);
	fb_info("Start to connect database.");
	if(!mysql_real_connect(connector, host,
			       user, passwd, db, port, NULL, 0)) {
		snprintf(buf, MAX_BUF, 
			"Failed to connect to database: %s",
			mysql_error(connector)); 
		fb_err(EXIT_FAILURE, buf);
	}

	//Load the configures from database.
	if( 0 != mysql_query(connector, "SELECT * FROM firebot_config"))
	fb_err(EXIT_FAILURE, mysql_error(connector));

	    res = mysql_store_result(connector);
	    if (!res)
		fb_err(EXIT_FAILURE, mysql_error(connector));
    
	while ((row = mysql_fetch_row(res)) != NULL) {
		/* row[0] stands for id
		 * row[1] stands for name
		 * row[3] stands for value
		 */
		if (GetConfig(row[1]) == NULL)
			AddConfig(row[1], row[2]);
		 else
		UpdateConfig(row[1], row[2]);
	}
	return 0;
}


