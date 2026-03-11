/**
 * @file dispatch.c
 * @author Logan Bakken
 * @brief This library implements functions to utilize poll() for the 
 * dispatching of fds to a thread pool. The "returned array" is a shared 
 * resource with worker threads.
 * @version 0.2
 * @date 2026-02-19
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "dispatch.h"

/**
 * @brief Function to initialize the fd dispatcher
 * 
 * @param num size of the management array
 * @return pointer to manager on success
 */
dispatch_manager_t *dispatch_init(size_t num)
{
    dispatch_manager_t *p_manager = NULL; 

    if (0 == num)
    {
        num = 8;
    }

    p_manager = calloc(1, sizeof(dispatch_manager_t));
    
    if (NULL == p_manager)
    {
        L_ERR("Calloc error: dispatch_init");
        return NULL;
    }

    p_manager->p_fds = calloc(num, sizeof(struct pollfd));
    
    if (NULL == p_manager->p_fds)
    {
        L_ERR("Calloc error: dispatch_init failed to allocate \
            pollfd array");
        free(p_manager);
        return NULL;
    }

    p_manager->fd_size = num;
    p_manager->fd_count = 0;
    
    p_manager->p_returned = calloc(MAX_CONNECTIONS, sizeof(int));

    if (NULL == p_manager->p_returned)
    {
        L_ERR("Calloc error: dispatch_init failed to allocate \
            returned array");
        free(p_manager->p_fds);
        free(p_manager);
        return NULL;
    }

    p_manager->return_size = MAX_CONNECTIONS;
    p_manager->return_count = 0;
    pthread_mutex_init(&(p_manager->dispatch_mutex), NULL);

    L_SUCCESS("Created dispatch manager");
    return p_manager;
}

/**
 * @brief Function to destroy the fd dispatcher
 * 
 * @param p_manager 
 */
void dispatch_destroy(dispatch_manager_t *p_manager)
{
    int i = 0;

    if (NULL == p_manager) 
    {
        return;
    }

    if (p_manager->p_fds)
    {
        for (i = 0; i < p_manager->fd_count; i ++)
        {
            if (0 <= p_manager->p_fds[i].fd)
            {
                close(p_manager->p_fds[i].fd);
            }
        }
        free(p_manager->p_fds);
    }

    if (p_manager->p_returned)
    {
        for (i = 0; i < p_manager->return_count; i ++)
        {
            if (0 <= p_manager->p_returned[i])
            {
                close(p_manager->p_returned[i]);
            }
        }
        free(p_manager->p_returned);
    }

    pthread_mutex_destroy(&(p_manager->dispatch_mutex));
    free(p_manager);
    L_SUCCESS("Destroyed dispatch manager");
}
/**
 * @brief Function to add a fd to the dispatcher
 * 
 * @param p_manager pointer to the management array
 * @param new_fd new fd to add
 */
void add_to_fds(dispatch_manager_t *p_manager, int new_fd)
{
    size_t new_size = 0;
    struct pollfd *p_temp = NULL;
    
    if (p_manager->fd_count >= MAX_CONNECTIONS)
    {
        L_ERR("Maximum number of connections. fd: %d refused", \
            new_fd);
        close(new_fd);
        return;
    }

    if (p_manager->fd_count == p_manager->fd_size) 
    {
        new_size = p_manager->fd_size * 2;

        if (new_size > MAX_CONNECTIONS)
        {
            new_size = MAX_CONNECTIONS;
        }

        p_temp = realloc(p_manager->p_fds, sizeof(struct pollfd) * new_size);
        
        if (NULL == p_temp)
        {
            L_ERR("Realloc error: cannot expand fd array");
            return;
        }

        p_manager->p_fds = p_temp;
        p_manager->fd_size = new_size;
    }

    p_manager->p_fds[p_manager->fd_count].fd = new_fd;
    p_manager->p_fds[p_manager->fd_count].events = POLLIN;
    p_manager->p_fds[p_manager->fd_count].revents = 0;
    p_manager->fd_count++;
}

/**
 * @brief Function to remove a fd from the dispatcher
 * 
 * @param p_manager pointer to the management array
 * @param i index of fd to remove
 */
void del_from_fds(dispatch_manager_t *p_manager, int i)
{
    p_manager->p_fds[i] = p_manager->p_fds[p_manager->fd_count - 1];
    p_manager->fd_count --;
    L_DBG("fd removed from array");
}

/**
 * @brief Function to handle new connections and give their fd to the dispatcher
 * 
 * @param listener listening socket for the server
 * @param p_manager pointer to the management array
 */
void handle_new_connection(int listener, dispatch_manager_t *p_manager)
{
    struct sockaddr_storage their_addr = {0};
    socklen_t addr_size = sizeof(their_addr);
    int new_fd = accept(listener, (struct sockaddr *)&their_addr, &addr_size);

    if (-1 == new_fd) 
    {
        L_ERR("Accept error");
    } 
    else 
    {
        add_to_fds(p_manager, new_fd);
        L_SUCCESS("New connection accepted");
    }
}

/**
 * @brief Function to process all connnections the dispatcher has (utilizing 
 * poll())
 * 
 * @param listener socket server is listening on
 * @param p_manager pointer to the management array
 * @return int -1 when nothing to process, fd when data to process
 */
int process_connections(int listener, dispatch_manager_t *p_manager)
{
    int ready_fd = -1;

    for (int i = 0; i < p_manager->fd_count; i++) 
    {
        if (p_manager->p_fds[i].revents & (POLLIN | POLLHUP | POLLERR)) 
        {
            if (p_manager->p_fds[i].fd == listener) 
            {
                handle_new_connection(listener, p_manager);
                p_manager->p_fds[i].revents = 0;
                continue; 
            } 
            else 
            {
                ready_fd = p_manager->p_fds[i].fd;
                del_from_fds(p_manager, i);
                return ready_fd;
            }
        }
    }

    return ready_fd;
}

/**
 * @brief Function to return fds back to the dispatcher. A mutex is included
 * because worker threads return fds when they finish a task. The mutex prevents
 * worker threads from concurrently writing to the same index in the return
 * array. 
 * 
 * @param p_manager pointer to the poll manager
 * @param client_fd fd to return
 */
void dispatch_return_fd(dispatch_manager_t *p_manager, int client_fd)
{
    pthread_mutex_lock(&(p_manager->dispatch_mutex));
    
    p_manager->p_returned[p_manager->return_count++] = client_fd;
    
    pthread_mutex_unlock(&(p_manager->dispatch_mutex));
}

/**
 * @brief Function to reintegrate returned fds in to the array
 * 
 * @param p_manager pointer to the poll manager
 */
void reintegrate_returned_fds(dispatch_manager_t *p_manager)
{
    for (int i = 0; i < p_manager->return_count; i ++)
    {
        add_to_fds(p_manager, p_manager->p_returned[i]);
    }

    p_manager->return_count = 0;
}

// End of file
