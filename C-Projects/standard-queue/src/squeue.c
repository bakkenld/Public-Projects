/**
 * @file squeue.c
 * @author Logan Bakken
 * @brief Function implementation for a Standard Queue 
 * Queue is operated operate FIFO, but the llist library has functions to 
 * accomodate LIFO/LILO/FILO. Suitable for any data type
 * Build upon a linked list. (llist.c)
 * @version 0.3
 * @date 2026-02-25
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../include/squeue.h"

/**
 * @brief Function to create the squeue.
 * 
 * @return squeue_t* NULL on failure, pointer to squeue on success. 
 */
squeue_t *squeue_create(void)
{
    return llist_create();
}

/**
 * @brief Function to create nodes for the squeue.
 * 
 * @param p_data_in pointer to the data to include.
 * @param data_size_in size of the data to injest. 
 * @return squeue_node_t* NULL on failure, pointer to the node on success. 
 */
squeue_node_t *node_create(void *p_data_in, size_t data_size_in);

/**
 * @brief Function to clear out the data in the squeue. 
 * 
 * @param pp_squeue pointer to the squeue to clear.
 * @return int8_t 0 on success, -1 on failure. 
 */
int8_t squeue_clear(squeue_t **pp_squeue)
{
    return llist_clear(pp_squeue);
}

/**
 * @brief Destroys the queue and frees the container pointer.
 * 
 * @param pp_squeue pointer to the squeue to destroy.
 */
int8_t squeue_destroy(squeue_t **pp_squeue)
{
    return llist_destroy(pp_squeue);
}

/**
 * @brief Function to push data to the queue. Utilizes llist library to add to 
 * the end of a linked list. 
 * 
 * @param p_squeue pointer to the queue. 
 * @param p_new_node pointer to the new node to add. 
 * @return int8_t 0 on success, 0 on failure
 */
int8_t squeue_push(llist_t *p_squeue, llist_node_t *p_new_node)
{
    return llist_add_last(p_squeue, p_new_node);
}

/**
 * @brief Function to peek the next data in the queue. Utilized llist library to
 * peek the first data. 
 * 
 * @param p_squeue pointer to the queue.
 * @param p_size_out pointer to a shared data buffer with the caller.
 * @return void* pointer to the peeked data. 
 */
void *squeue_peek(llist_t *p_squeue, size_t *p_size_out)
{
    return llist_peek_first(p_squeue, p_size_out);
}

/**
 * @brief Function to pop the next data in the queue. Utilizes llist library to 
 * pop the first data
 * 
 * @param p_squeue pointer to the queue.
 * @param p_size_out pointer to a shared data buffer with the caller.
 * @return void* pointer to the popped data.  
 */
void *squeue_pop(llist_t *p_squeue, size_t *p_size_out)
{
    return llist_pop_first(p_squeue, p_size_out);
}

// End of file
