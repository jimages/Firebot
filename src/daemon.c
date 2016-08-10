/*
 * Date:	Aug.1 2016
 * Author:	Zachary Wang(jimages123@gmail.com)
 * Copyright(c) 2016 Under MIT License
 * See the full license in the root directory.
 */

/*
 * Manage the daemon.
 */
#include"include/firebot.h"

static pid_t daemon_id;

bool StartDaemon()
{
	extern pid_t daemon_id;
	FILE *file;

	fb_info( "Start daemon.");
	int res = fork();
	if (res == -1)
		fb_err(EXIT_FAILURE, strerror(errno));
	if (res != 0)
		exit(0);
	daemon_id = setsid();
	if (daemon_id == (pid_t)-1) fb_err(EXIT_FAILURE, strerror(errno));
	res = chdir("/");
	if (res == -1) fb_err(EXIT_FAILURE, strerror(errno));

	// Write PID to file.
	fb_info("Try write PID to the file.");
	file = fopen("/var/run/firebot.pid","w");
	if (!file) fb_err(EXIT_FAILURE, strerror(errno));
	if (!fwrite(&daemon_id, sizeof(pid_t), 1, file))
		fb_err(EXIT_FAILURE, strerror(errno));
	fclose(file);
	return 1;
}
