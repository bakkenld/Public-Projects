/**
 * @file cllist.c
 * @author Logan Bakken
 * @brief Funciton definitions for a circularly linked list.
 * Suitable for any data type.  
 * Built upon a linked list
 * @version 0.2
 * @date 2026-02-27
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../include/cllist.h"

/**
 * @brief Helper function to de-link the circle for llist functions that 
 * traverse the list. 
 * 
 * @param p_cllist pointer to the cllist
 * @return int8_t -1 on failure, 0 on success. 
 */
static int8_t de_circle(cllist_t *p_cllist)
{
    int8_t return_val = -1;

    if (NULL == p_cllist || NULL == p_cllist->p_tail) 
    {
        L_DBG("Cannot de-circle list");
        goto EXIT;
    }

    p_cllist->p_tail->p_next = NULL;
    return_val = 0;

EXIT:
    return return_val;
}

/**
 * @brief Function to "re-circulate" the cllist after utilizing a llist 
 * function that requires a linear list.
 * 
 * @param p_cllist pointer to the cllist
 * @return int8_t -1 on failure, 0 on success
 */
static int8_t re_circle(cllist_t *p_cllist)
{
    int8_t return_val = -1;
    
    if (NULL == p_cllist || NULL == p_cllist->p_head)
    {
        L_DBG("Unable to make list circular");
        goto EXIT;
    }

    p_cllist->p_tail->p_next = p_cllist->p_head;

    return_val = 0;

EXIT: 
    return return_val;
}

/**
 * @brief Wrapper function for llist logic. 
 * 
 * @return cllist_t* pointer to the circ-linked-list. 
 */
cllist_t *cllist_create(void)
{
    return llist_create();
}

/**
 * @brief Function to clear a circularly linked list. The circle is "broken"
 * then the list is cleared. 
 * 
 * @param pp_cllist pointer to the circ linked list to destroy.
 * @return int8_t 0 on success, -1 on failure
 */
int8_t cllist_clear(cllist_t *p_cllist)
{
    de_circle(p_cllist);

    return llist_clear(&p_cllist);
}

/**
 * @brief Function to destroy a circularly linked list. The circle is "broken"
 * then the list is destroyed. 
 * 
 * @param pp_cllist pointer to the circ linked list to destroy.
 * @return int8_t 0 on success, -1 on failure
 */
int8_t cllist_destroy(cllist_t **pp_cllist)
{
    de_circle(*pp_cllist);

    return llist_destroy((llist_t **)pp_cllist);
}

/**
 * @brief Function to add to the front of the linked list, and adjust the 
 * tail/head pointers to make the list circular. 
 * 
 * @param p_cllist pointer to the cllist
 * @param p_new_node pointer to the node to add. 
 * @return int8_t -1 on failure, 0 on success. 
 */
int8_t cllist_add_first(cllist_t *p_cllist, cllist_node_t *p_new_node)
{
    int8_t return_val = -1;

    return_val = llist_add_first(p_cllist, p_new_node);

    if (0 == return_val)
    {
        return_val = re_circle(p_cllist);
    }

    return return_val;
}

/**
 * @brief Function to add to the end of the cllist. re_circle is called 
 * since the llist library sets tail->next = NULL when inserting nodes to the 
 * tail. 
 * 
 * @param p_cllist pointer to the cllist
 * @param p_new_node pointer to the node to add. 
 * @return int8_t -1 on failure, 0 on success. 
 */
int8_t cllist_add_last(cllist_t *p_cllist, cllist_node_t *p_new_node)
{
    llist_add_last(p_cllist, p_new_node);
    return re_circle(p_cllist);
}

/**
 * @brief Wrapper function for llist logic. Uses re_circle to ensure the tail
 * points to the new head of the list. 
 * 
 * @param p_cllist pointer to the cllist
 * @return int8_t -1 on failure, 0 on success. 
 */
int8_t cllist_delete_first(cllist_t *p_cllist)
{
    int8_t return_val = llist_delete_first(p_cllist);
    if (0 == return_val && 0 < p_cllist->num_nodes)
    {
        re_circle(p_cllist);
    }

    return return_val;
}

/**
 * @brief Function to delete from the end of the cllist. re_circle is called 
 * since the llist library sets tail->next = NULL when deleting the tail node. 
 * 
 * @param p_cllist pointer to the cllist
 * @param p_new_node pointer to the node to add. 
 * @return int8_t -1 on failure, 0 on success. 
 */
int8_t cllist_delete_last(cllist_t *p_cllist)
{
    de_circle(p_cllist);
    llist_delete_last(p_cllist);
    return re_circle(p_cllist);
}

/**
 * @brief Wrapper function for llist logic. 
 */
void *cllist_peek_first(cllist_t *p_cllist, size_t *p_size_out)
{
    return llist_peek_first(p_cllist, p_size_out);
}

/**
 * @brief Wrapper function for llist logic. 
 */
void *cllist_peek_last(cllist_t *p_cllist, size_t *p_size_out)
{
    return llist_peek_last(p_cllist, p_size_out);
}

/**
 * @brief Wrapper function for llist logic. Uses re_circle to point the tail to 
 * the new head. 
 */
void *cllist_pop_first(cllist_t *p_cllist, size_t *p_size_out)
{
    void *p_data = llist_pop_first(p_cllist, p_size_out);

    if (NULL != p_data && 0 < p_cllist->num_nodes)
    {
        re_circle(p_cllist);
    }
    return p_data;
}

/**
 * @brief Function to delete from the end of the cllist. re_circle is called 
 * since the llist library sets tail->next = NULL when deleting the tail node. 
 * 
 * @param p_cllist pointer to the cllist
 * @param p_size_out pointer to shared output data buffer. 
 * @return coid * to the returned data. 
 */
void *cllist_pop_last(cllist_t *p_cllist, size_t *p_size_out)
{
    de_circle(p_cllist);
    void *p_data = llist_pop_last(p_cllist, p_size_out);

    if (p_cllist->num_nodes > 0) 
    {
        re_circle(p_cllist);
    }
    return p_data;
}

/**
 * @brief Wrapper function for llist logic. Index check is done to utilize 
 * add_first and add_last de/re_circle logic implementation. 
 */
int8_t cllist_add_index(cllist_t *p_cllist, cllist_node_t *p_new_node, 
    int index_in)
{
    if (0 == index_in)
    {
        return cllist_add_first(p_cllist, p_new_node);
    }

    else if (index_in == p_cllist->num_nodes)
    {
        return cllist_add_last(p_cllist, p_new_node);
    }

    else 
    {
    return llist_add_index(p_cllist, p_new_node, index_in);
    }
}

/**
 * @brief Wrapper function for llist logic. 
 */
int8_t cllist_delete_index(cllist_t *p_cllist, int index_in)
{
    return llist_delete_index(p_cllist, index_in);
}

/**
 * @brief Wrapper function for llist logic. 
 */
void *cllist_peek_index(cllist_t *p_cllist, int index_in, size_t *p_size_out)
{
    return llist_peek_index(p_cllist, index_in, p_size_out);
}

/**
 * @brief Wrapper function for llist logic. 
 */
void *cllist_pop_index(cllist_t *p_cllist, int index_in, size_t *p_size_out)
{
    return llist_pop_index(p_cllist, index_in, p_size_out);
}

// End of file
