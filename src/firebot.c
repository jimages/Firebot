/*
 * Date:    May.18 2016
 * Author:  Zahcary Wang(jimages123@gmail.com)
 * Copyright(c) 2016   under  MIT License
 * See the full license file in root directory.
 */
#include "include/firebot.h"

int main(int argc, char *argv[])
{


	// Initialize configure module.
	ConfigInit(argc, argv);

	// Init log module.
	LogInit();

	fb_info("firebot started.");

	// A one-node crawler first
	//LoadNetworkConfig();

	curl_global_init(CURL_GLOBAL_SSL);

	daemon_init();

	return 0;
}
