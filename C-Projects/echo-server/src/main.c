/**
 * @file main.c
 * @author Logan Bakken
 * @brief Implementation of an echo server
 * @version 0.1
 * @date 2026-02-04
 * 
 * @copyright Copyright (c) 2026
 * TODO: tpool, eserver, main corrections
 * TODO: respond to merge request directly
 * max connections currently 256
 */

#include "../include/eserver.h"

volatile sig_atomic_t keep_running = 1; 

/**
 * @brief Function for graceful shutdown
 * 
 * @param sig SIGINT
 */
void handle_sigint(int sig) 
{
    L_ERR("SIGINT received");
    (void)sig;
    keep_running = 0; 
}

int main(int argc, char *argv[])
{
    int listen_fd = 0;
    server_config_t config = {0};
    struct sigaction sa = {0};

    // signal handling
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGPIPE, SIG_IGN);

    // init phase
    listen_fd = server_init(argc, argv, &config);
    
    if (-1 == listen_fd)
    {
        L_ERR("Incorrect start-up attempt");
        return 0;
    }
    
    tpool_t *p_tm = tpool_create(config.num_threads);
    
    if (NULL == p_tm)
    {
        L_ERR("NULL: p_tm");
        goto EXIT;
    }

    dispatch_manager_t *p_manager = dispatch_init(config.dispatch_size);

    if (NULL == p_manager)
    {
        L_ERR("NULL: p_manager");
        goto EXIT;
    }

    // Run phase
    run_server(listen_fd, p_tm, p_manager);

    // teardown phase
EXIT: 
    tpool_destroy(p_tm);
    dispatch_destroy(p_manager);
    return 0;
}

// End of file
