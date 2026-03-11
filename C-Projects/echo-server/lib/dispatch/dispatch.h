/**
 * @file dispatch.h
 * @author Logan Bakken
 * @brief header file for dispatch.c
 * @version 0.1
 * @date 2026-01-31
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef dispatch_H
#define dispatch_H

#include <netdb.h>
#include <poll.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "logger.h"

#define MAX_CONNECTIONS 256

typedef struct dispatch_manager {
    struct pollfd *p_fds;
    int fd_count;
    int fd_size;
    int *p_returned;
    int return_count;
    int return_size;
    pthread_mutex_t dispatch_mutex; // Worker threads return fds 
} dispatch_manager_t;

typedef struct client_task {
    int client_fd;
    dispatch_manager_t *p_manager;
} client_task_t;

// Life cycle operations:
dispatch_manager_t *dispatch_init(size_t num);
void dispatch_destroy(dispatch_manager_t *p_manager);

// Array maintanence:
void add_to_fds(dispatch_manager_t *p_manager, int new_fd);
void del_from_fds(dispatch_manager_t *p_manager, int i);

// Poll processing:
void handle_new_connection(int listener, dispatch_manager_t *p_manager);
int process_connections(int listener, dispatch_manager_t *p_manager);
void dispatch_return_fd(dispatch_manager_t *p_manager, int client_fd);
void reintegrate_returned_fds(dispatch_manager_t *p_manager);

#endif // dispatch_H

// End of file
