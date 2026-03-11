/**
 * @file eserver.c
 * @author Logan Bakken
 * @brief Functions for an echo-server
 * @version 0.1
 * @date 2026-01-28
 * 
 * @copyright Copyright (c) 2026
 */

 #include "../include/eserver.h"

 extern volatile sig_atomic_t keep_running;

 /**
  * @brief Funtion to initialize the server
  * 
  * @param p_config pointer to config struct (located in startup.c/.h)
  * @return int sock descriptor on success, -1 on error
  */
int server_init(int argc, char *argv[], server_config_t *p_config) 
{
    int sockfd = -1;
    int status = 0;
    int yes = 1; // To avoid REUSEADDR 
    struct addrinfo hints = {0};
    struct addrinfo *p_res = NULL;
    struct addrinfo *p_scout = NULL; // for portability

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE;     

    int start_server = recv_input(argc, argv, p_config);
    if (0 != start_server)
    {
        if (-1 == start_server)
        {
            L_DBG("Incorrect start-up attempted");
            printf("Usage: %s [-p port] [-h (help)]\n", argv[0]);
            goto EXIT;
        }
        if (1 == start_server)
        {
            L_DBG("Client used -h");
            goto EXIT;
        }
    }

    status = getaddrinfo(NULL, p_config->p_myport, &hints, &p_res);

    if(0 != status)
    {
        L_ERR("getaddrinfo: %s", gai_strerror(status));
        goto EXIT;
    }

    for (p_scout = p_res; p_scout != NULL; p_scout = p_scout->ai_next) 
    {
        if ((sockfd = socket(p_scout->ai_family, p_scout->ai_socktype, \
            p_scout->ai_protocol)) == -1) 
        {
            L_DBG("Socket failed");
            continue;
        }

        // Prevents "Address already in use" error on restart
        if (0 != setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, \
            sizeof(int))) 
        {
            close(sockfd);
            freeaddrinfo(p_res);
            L_ERR("setsockopt failed");
            goto EXIT;
        }

        if (0 != bind(sockfd, p_scout->ai_addr, p_scout->ai_addrlen)) 
        {
            close(sockfd);
            L_DBG("Socket bound");
            continue;
        }

        L_SUCCESS("Socket bound on port: %s", p_config->p_myport);
        break;
    }

    freeaddrinfo(p_res); 

    if (NULL == p_scout) 
    {
        L_ERR("Server: failed to bind to any address");
        goto EXIT;
    }

    if (0 != listen(sockfd, p_config->waitlist)) 
    {
        L_ERR("Listen failed");
        close(sockfd);
        goto EXIT;
    }

EXIT:
    return sockfd;
}

/**
 * @brief Function to echo back to client
 * 
 * @param p_arg pointer to the client to echo back to
 */
void handle_client_data(void *p_arg)
{
    char buffer[1024] = {0};
    int client_fd = 0;
    ssize_t bytes_received = 0;
    ssize_t bytes_sent = 0;
    client_task_t *p_task = NULL;
    dispatch_manager_t *p_manager = NULL;
    
    if (NULL == p_arg) 
    {
        L_ERR("Handle_client_data NULL parameter: p_arg");
        goto EXIT;
    }

    p_task = (client_task_t *)p_arg;
    p_manager = p_task->p_manager;
    client_fd = p_task->client_fd;
    free(p_arg);
    L_DBG("Worker thread handling task");
    
    bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (0 < bytes_received)
    {
        buffer[bytes_received] = '\0';

        L_SUCCESS("Revieved data from client: %d", client_fd);
        printf("Recieved from client %d:\n%s", client_fd, buffer);

        bytes_sent = send(client_fd, buffer, bytes_received, 0);
        L_DBG("Attempting to echo to client %d...", client_fd);

        if (0 > bytes_sent) 
        {
            L_ERR("Send failed");
            goto EXIT;
        }

        L_SUCCESS("Echoed to client %d", client_fd);
        printf("Echoing to client %d:\n%s", client_fd, buffer);
        dispatch_return_fd(p_manager, client_fd);
        L_DBG("Returning task to dispatcher");
        return;
    }

    if (0 > bytes_received) 
    {
        L_ERR("Recv failed");
        goto EXIT;
    } 
    
    else if (0 == bytes_received) 
    {
        L_SUCCESS("Client %d disconnected", client_fd);
        goto EXIT;
    }

EXIT:
    close(client_fd);
}

/**
 * @brief Function to run the echo server
 * 
 * @param listen_fd the bound socket passed from server_init 
 * @param p_tm pointer to the thread pool
 * @param p_manager pointer to the dispatch manager
 */
void run_server(int listen_fd, tpool_t *p_tm, dispatch_manager_t *p_manager)
{
    int dispatch_count = 0;
    int ready_client = -1;
    client_task_t *p_task = NULL;

    add_to_fds(p_manager, listen_fd);

    L_SUCCESS("Server is listening for connections");
    printf("Server is listening for connections...\n");

    while (keep_running) 
    {
        reintegrate_returned_fds(p_manager);
        dispatch_count = poll(p_manager->p_fds, p_manager->fd_count, 500);
    
        if (0 < dispatch_count) 
        {
            // Call process until it returns -1 (handling all ready events)
            while (-1 != (ready_client = process_connections(listen_fd, \
                p_manager)))
            {
                p_task = calloc(1, sizeof(client_task_t));
                
                if (NULL == p_task)
                {
                    L_ERR("p_task Calloc error");
                    close(ready_client);
                    continue;
                }
                p_task->client_fd = ready_client;
                p_task->p_manager = p_manager;
                
                if (!tpool_add_work(p_tm, handle_client_data, p_task))
                {
                    L_ERR("Failed to dispatch FD %d", \
                        ready_client);
                    close(ready_client);
                    free(p_task);
                }
            }
        }
    }
    L_SUCCESS("Exiting and initiating clean up");
}

// End of file
