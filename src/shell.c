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

static inline int out_help();
static inline int out_copyright();
static inline int out_version();

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


