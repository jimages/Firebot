/*
 * Date:    May.18 2016
 * Author:  Zahcary Wang(jimages123@gmail.com)
 * Copyright(c) 2016   under  MIT License
 * See the full license file in root directory.
 */
#include "include/firebot.h"

int main(int argc, const char ** argv) {
    // Intialize mysql library.
    mysql_library_init(0, NULL, NULL);

    // Initialize configure module.
    ConfigInit();

    // Init log module.
    LogInit();

    Log(0, log_info, "firebot started.");

    LoadNetworkConfig();

    curl_global_init(CURL_GLOBAL_SSL);

    return 0;
}
