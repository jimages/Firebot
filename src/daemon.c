/*
 * Date:	Aug.1 2016
 * Author:	Zachary Wang(jimages123@gmail.com)
 * Copyright(c) 2016 Under MIT License
 * See the full license in the root directory.
 */

/*
 * Manage the daemon.
 */
#include "include/firebot.h"
#include <sys/types.h>
#include <signal.h>

#define PID_FILE_PATH "/var/run/firebot.pid"

static pid_t daemon_id;

static int start_daemon();
static int stop_daemon();
static int restart_daemon();
static int load_pid();

bool daemon_init()
{
	const char *conf;

	conf = GetConfig("DAEMON_MODE");
	if (conf == NULL)
		return 1;

	load_pid();
	// Before fork we shoud flush everything.
	fflush(NULL);
	
	if (strcasecmp(conf, "START") == 0)
		start_daemon();

	if (strcasecmp(conf, "STOP") == 0)
		stop_daemon();

	if (strcasecmp(conf, "RESTART") == 0)
		restart_daemon();

	exit(EXIT_SUCCESS);
	return 1;
}	

static int start_daemon()
{
	extern pid_t daemon_id;
	FILE *file;
	int res;

	fb_info( "Start daemon.");

	if (daemon_id != (pid_t)0)
		fb_err(EXIT_FAILURE, "A daemon is already running.");

	res = fork();
	if (res == -1) 
		fb_err(EXIT_FAILURE, "Can't start daemon: %s", 
		       strerror(errno));

	if (res != 0) {
		sleep(5);
		exit(0);
	}
	daemon_id = setsid();
	if (daemon_id == (pid_t)-1) fb_err(EXIT_FAILURE, strerror(errno));
	res = chdir("/");
	if (res == -1) fb_err(EXIT_FAILURE, strerror(errno));

	// Write PID to file.
	fb_info("Try write PID to the file.");
	file = fopen(PID_FILE_PATH,"w");
	if (!file) fb_err(EXIT_FAILURE, strerror(errno));
	if (!fwrite(&daemon_id, sizeof(pid_t), 1, file))
		fb_err(EXIT_FAILURE, strerror(errno));
	fclose(file);
	return 0;
}

static int stop_daemon()
{
	int res;
	extern pid_t daemon_id;

	fb_info("Stop daemon.");
	if (daemon_id == (pid_t)0)
		fb_err(EXIT_FAILURE, "Daemon is not running.");
	
	res = kill(daemon_id, SIGTERM);	
	if (0 != res)
		fb_err(EXIT_FAILURE, "Can't stop daemon: %s",
		       strerror(errno));
	res = remove(PID_FILE_PATH);
	if (res != 0)
		fb_err(EXIT_FAILURE, "Can't remove pid file: %s : %s",
		       PID_FILE_PATH, strerror(errno));
	return 0;
}	

static int restart_daemon()
{
	fb_info("Restart daemon");
	stop_daemon();
	fb_info("Daemon stoped.");
	start_daemon();
	fb_info("Daemon started.");
	return 0;
}

static int load_pid()
{
	extern pid_t daemon_id;
	FILE *file;
	int res;

	res = access(PID_FILE_PATH, F_OK);
       	if (res != 0) {
		fb_info("Pid file does not exist.");
		return 0;
	}

	res = access(PID_FILE_PATH, R_OK);
	if (res != 0) {
		fb_err(EXIT_FAILURE, "%s : %s", LOG_FILE_PATH ,
			 strerror(errno));
		return -1;
	}
	file = fopen(LOG_FILE_PATH, "r");
	res = fscanf(file, "%d", &daemon_id);
	if (res != 1)
		fb_warning("Can't read pid from file: %s",
		       strerror(errno));	
	return 0;
}
