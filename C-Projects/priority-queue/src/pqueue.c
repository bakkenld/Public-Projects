/**
 * @file pqueue.c
 * @author Logan Bakken
 * @brief Function implementation for a Priority Queue 
 * Queue is operated operate FIFO, but the llist library has functions to 
 * accomodate LIFO/LILO/FILO. Suitable for any data type
 * Build upon a linked list. (llist.c)
 * @version 0.3
 * @date 2026-02-27
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../include/pqueue.h"

/**
 * @brief Function to create the pqueue.
 * 
 * @return pqueue_t* NULL on failure, pointer to pqueue on success. 
 */
pqueue_t *pqueue_create(void)
{
    return llist_create();
}

/**
 * @brief Function to clear out the data in the pqueue. 
 * 
 * @param pp_pqueue pointer to the pqueue to clear.
 * @return int8_t 0 on success, -1 on failure. 
 */
int8_t pqueue_clear(pqueue_t **pp_pqueue)
{
    return llist_clear(pp_pqueue);
}

/**
 * @brief Destroys the queue and frees the container pointer.
 * 
 * @param pp_pqueue pointer to the pqueue to destroy.
 */
int8_t pqueue_destroy(pqueue_t **pp_pqueue)
{
    return llist_destroy(pp_pqueue);
}

/**
 * @brief Function to push data to the queue. Utilizes llist library to add to 
 * the end of a linked list. 
 * 
 * @param p_pqueue pointer to the queue. 
 * @param p_new_node pointer to the new node to add. 
 * @return int8_t 0 on success, -1 on failure
 */
int8_t pqueue_push(pqueue_t *p_pqueue, void *p_data_in, size_t data_size_in, \
    int8_t priority_in) 
{
    int8_t return_val = -1;
    pqueue_item_t item = 
    {
        .priority = priority_in, 
        .p_data = p_data_in,
        .data_size = data_size_in
    };

    pqueue_node_t *p_new_node = node_create(&item, sizeof(pqueue_item_t));

    if (NULL == p_new_node)
    {
        L_DBG("Failed to create new node");
        goto EXIT;
    }
    
    int index = 0;
    pqueue_node_t *p_current = p_pqueue->p_head;

    while (p_current != NULL) 
    {
        pqueue_item_t *p_current_data = (pqueue_item_t *)p_current->p_data;
        if (priority_in < p_current_data->priority) 
        {
            break;
        }
        p_current = p_current->p_next;
        index++;
    }

    return_val = llist_add_index(p_pqueue, p_new_node, index);

EXIT:
    return return_val;
}

/**
 * @brief Function to peek the next data in the queue. Utilized llist library to
 * peek the first data. 
 * 
 * @param p_pqueue pointer to the queue.
 * @param p_size_out pointer to a shared data buffer with the caller.
 * @return void* pointer to the peeked data. 
 */
void *pqueue_peek(llist_t *p_pqueue, size_t *p_size_out)
{
    return llist_peek_first(p_pqueue, p_size_out);
}

/**
 * @brief Function to pop the next data in the queue. Utilizes llist library to 
 * pop the first data
 * 
 * @param p_pqueue pointer to the queue.
 * @param p_size_out pointer to a shared data buffer with the caller.
 * @return void* pointer to the popped data.  
 */
void *pqueue_pop(llist_t *p_pqueue, size_t *p_size_out)
{
    return llist_pop_first(p_pqueue, p_size_out);
}

// End of file
