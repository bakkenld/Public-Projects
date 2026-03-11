/**
 * @file eserver.h
 * @author Logan Bakken
 * @brief header for eserver.c
 * @version 0.1
 * @date 2026-01-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef ESERVER_H
#define ESERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include "logger.h"
#include "tpool.h"
#include "dispatch.h"
#include "startup.h"

extern volatile sig_atomic_t keep_running; 

int server_init(int argc, char *argv[], server_config_t *p_config);
void handle_client_data(void *p_arg);
void run_server(int listen_fd, tpool_t *p_tm, dispatch_manager_t *p_manager);
void handle_sigint(int sig);

#endif // ESERVER_H

// End of file 
