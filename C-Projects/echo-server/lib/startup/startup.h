/**
 * @file sinput.h
 * @author Logan Bakken
 * @brief Handling user input to start echo-server
 * @version 0.1
 * @date 2026-02-06
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef SINPUT_H
#define SINPUT_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct server_config {
    char   *p_myport;
    int    waitlist;
    int    num_threads;
    int    dispatch_size;
} server_config_t;

int recv_input(int argc, char *argv[], server_config_t *p_config);

#endif // SINPUT_H

// End of file
