/*
 * Data:	Aug.8 2016
 * Author:	Zachary Wang(jimages123@gmail.com)
 * Copyright(c) 2016 Under MIT License
 * See the full license in root directory.
 */

/*
 * parsing flags
 */
#include "include/firebot.h"

const char firebot_usage_string[] =
	"Usage: firebot [-hv] [-c <path>] [-l <path>]\n"
	"[-d [start | stop | restart]\n\n"
	"Options:\n"
	"	-h		Print this help.\n"
	"	-v		Print the version.\n"
	"	-c <path>	Specify the configure file.\n"
	"	-l <path>	Specify the log file.\n"
	"	-d [start | stop | restart] daemon mode.start if without command.\n\n"
	"Report bugs: <https://github/jimages/Firebot/issues>\n";

const char firebot_copyright_string[] =
	"Firebot (A Multi-threads discrete spider)\n"
	"Copyright(C) Zachary Wang(jimages123@gmail.com\n"
	"Under MIT LIcense <https://opensource.org/license/MIT>\n"
	"There is NO WARRANTY, to the exten permitted by law\n\n";

const char firebot_version_string[] =
	"Firebot " FIREBOT_VERSION "\n";

const char optstring[] = ":hvc:l:d:";

const struct flag_map {
	char flag;
	const char *conf;
} flags_map[] = {
		{'c', "CONFIG_FILE_PATH"},
		{'l', "LOG_FILE_PATH"},
		{'d', "DAEMON_MODE"}
};

static inline int out_help();
static inline int out_copyright();
static inline int out_version();
static const char *get_conf(char c);

static inline int out_help()
{
	extern const char firebot_usage_string[];

	out_copyright();
	fputs(firebot_usage_string, stderr);
	return 0;
}

static inline int out_copyright()
{
	extern const char firebot_copyright_string[];

	fputs(firebot_copyright_string, stderr);
	return 0;
}

static inline int out_version()
{
	extern const char firebot_version_string[];

	out_copyright();
	fputs(firebot_version_string, stderr);
	return 0;
}

static inline int valid_daemon_command(const char *command) {
	bool res = false;
	const char *commands[3] = { "start", "stop", "restart" };

	for(int i = 0 ; i < 3 ; i++)
		res |= (strcasecmp(command, commands[i]) == 0);
	/*
	 * When res is 0(false) it means no one of the results
	 * of comparisons is 0. In other words the command is
	 * a invalid command.
	 */
	if (res == true)
		return 0;
	else
		return -1;
}

static const char *get_conf(char c) {
	extern const struct flag_map flags_map[];

	size_t maxind = sizeof(flags_map) / sizeof(flags_map[0]) - 1;
	for (size_t i = 0 ; i <= maxind ; i++) {
		if (flags_map[i].flag == c)
			return flags_map[i].conf;
	}
	return NULL;
}

/*
 * Here we load flags and set configure for these.
 */
bool shell_init(int argc, char *argv[])
{
	char c;
	extern const char optstring[];
	if (argc <= 1) {
		out_help();
		exit(EXIT_SUCCESS);
	}

	while (1) {
		const char *conf;

		c = getopt(argc, argv, optstring);
		if (c == (char)-1) break;
		switch (c) {
		case 'v':
			out_version();
			exit(EXIT_SUCCESS);
		case 'h':
			out_help();
			exit(EXIT_SUCCESS);
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
				Log(EXIT_FAILURE, log_error,
				    "Invalid daemon command.");
			AddConfig(conf, optarg);
			break;
		case ':':
			Log(0, log_warning, "Missing option argument.\n");
			out_help();
			exit(EXIT_FAILURE);
			break;
		default: // for '?'
			if (errno != 0)
				Log(EXIT_FAILURE, log_error,
				    strerror(errno));
			fprintf(stderr, "Unrecognized option -%c.\n", c);
			out_help();
			Log(EXIT_FAILURE, log_error,
			    "Unrecognized option.\n");
			break;
		}
	}
	return 0;
}
