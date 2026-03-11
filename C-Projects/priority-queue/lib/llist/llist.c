/**
 * @file llist.c
 * @author Logan Bakken
 * @brief Funciton definitions for a singly linked list
 * Suitable for any data type. This list moves data on to the stack. Macro 
 * guards are used to trim out unused functions for various data structures
 * 
 * @version 0.3
 * @date 2026-02-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "llist.h"

/**
 * @brief Function to create a node in a linked list. The node copies the data 
 * directly to the heap so that the node can "own" the memory
 * 
 * @param p_data_in data to include in the node
 * @param data_size_in size of the data to include
 * @return llist_node_t* pointer to the node on success, NULL on failure
 */
llist_node_t *node_create(void *p_data_in, size_t data_size_in)
{
    llist_node_t *p_new_node = NULL;

    if (NULL == p_data_in || 0 == data_size_in)
    {
        L_DBG( "Invalid parameters for node creation ");
        goto EXIT;
    }

    p_new_node = calloc(1, sizeof(llist_node_t));
    if (NULL == p_new_node)
    {
        L_DBG( "llist calloc failure ");
        goto EXIT;
    }

    p_new_node->p_data = calloc(1, data_size_in);
    if (NULL == p_new_node->p_data)
    {
        L_DBG( "llist calloc failure ");
        free(p_new_node);
        goto EXIT;
    }
    
    memcpy(p_new_node->p_data, p_data_in, data_size_in);
    p_new_node->data_size = data_size_in;
    p_new_node->p_next = NULL;
    L_DBG( "Created new node ");

EXIT:
    return p_new_node;
}

/**
 * @brief Helper function for deleting nodes. Frees the data within the node.
 * Does not handle pointers around the node.
 * 
 * @param p_node_to_del pointer to the node to delete.
 * @return int8_t -1 on failure, 0 on success
 */
int8_t node_destroy(llist_node_t *p_node_to_des)
{
    int8_t return_val = -1;

    if (NULL == p_node_to_des)
    {
        L_DBG( "Bad parameter, cannot destroy node ");
        goto EXIT;
    }

    free(p_node_to_des->p_data);
    p_node_to_des->p_data = NULL;
    free(p_node_to_des);
    return_val = 0;

EXIT: 
    return return_val;
}

/**
 * @brief Function to create an empty list
 * 
 * @return llist_t* pointer to list location on success, NULL on failure
 */
llist_t *llist_create(void)
{
    llist_t *p_new_llist = NULL;
    
    p_new_llist = calloc(1, sizeof(llist_t));
    
    if (NULL == p_new_llist)
    {
        L_DBG("llist calloc failure ");
        goto EXIT;
    }

    L_DBG( "Created new llist ");

EXIT:
    return p_new_llist;
}

/**
 * @brief Function to clear out the data in a linked list while maintaining the
 * structure. 
 * 
 * @param pp_llist pointer to the linked list to clear
 * @return int returns -1 on failure, 0 on success
 */
int8_t llist_clear(llist_t **pp_llist)
{
    int8_t return_val = -1;

    if ((NULL == pp_llist) || (NULL == *pp_llist))
    {
        L_DBG("Bad parameter, unable to clear list ");
        goto EXIT;
    }

    llist_node_t *p_current = (*pp_llist)->p_head;
    llist_node_t *p_next = NULL;

    while (p_current != NULL)
    {
        p_next = p_current->p_next;
        node_destroy(p_current);
        p_current = p_next;
    }

    (*pp_llist)->p_head = NULL;
    (*pp_llist)->p_tail = NULL;
    (*pp_llist)->num_nodes = 0;

    return_val = 0;
    L_DBG( "Cleared llist");

EXIT:
    return return_val;
}

/**
 * @brief Function to destroy a linked list.
 * 
 * @param pp_llist pointer to the linked list to destroy
 * @return int8_t 0 on  success, -1 on failure
 */
int8_t llist_destroy(llist_t **pp_llist)
{
    int8_t return_val = -1;

    if ((NULL == pp_llist) || (NULL == *pp_llist))
    {
        L_DBG("Bad parameter, unable to destroy list ");
        goto EXIT;
    }

    if (0 == llist_clear(pp_llist))
    {
        free(*pp_llist);
        *pp_llist = NULL;
        return_val = 0;
        L_DBG( "Destroyed llist");
        goto EXIT;
    }

    L_DBG( "Could not destroy llist");

EXIT:
    return return_val;
}

/**
 * @brief Function to add a node to the end of a linked list. 
 * 
 * @param p_llist pointer to the linked list
 * @param p_new_node pointer to the node to add
 * @return -1 on failure, 0 on success
 */

int8_t llist_add_last(llist_t *p_llist, llist_node_t *p_new_node)
{
    int8_t return_val = -1;

    if ((NULL == p_llist) || (NULL == p_new_node))
    {
        L_DBG("Bad parameter, cannot add to linked list ");
        goto EXIT;
    }

    p_new_node->p_next = NULL;

    if (NULL == p_llist->p_head) 
    {
        p_llist->p_head = p_new_node;
        p_llist->p_tail = p_new_node;
    }
    
    else
    {
        p_llist->p_tail->p_next = p_new_node; 
        p_llist->p_tail = p_new_node; 
    }
    
    p_llist->num_nodes ++;
    return_val = 0; 
    L_DBG( "Added node to llist end ");

EXIT:
    return return_val;
}

/**
 * @brief Function to add node to front of a linked list. 
 * 
 * @param p_llist pointer to the linked list
 * @param p_new_node pointer to the new node to add
 * @return int returns -1 on failure, 0 on success
 */
int8_t llist_add_first(llist_t *p_llist, llist_node_t *p_new_node)
{
    int8_t return_val = -1;

    if ((NULL == p_llist) || (NULL == p_new_node))
    {
        L_DBG("Bad parameter, cannot add to linked list ");
        goto EXIT;
    }
    
    p_new_node->p_next = p_llist->p_head;
    p_llist->p_head = p_new_node;

    if (NULL == p_llist->p_tail)
    {
        p_llist->p_tail = p_new_node;
    }

    p_llist->num_nodes ++;
    return_val = 0;
    L_DBG( "Added node to llist front ");

EXIT:
    return return_val;
}

/**
 * @brief Function to delete the last node in a linked list.
 * 
 * @param p_llist pointer to the linked list
 * @return int returns -1 on failure, 0 on success
 */
int8_t llist_delete_last(llist_t *p_llist)
{
    int8_t return_val = -1;

    if ((NULL == p_llist) || (NULL == p_llist->p_head)) 
    {
        L_DBG("Bad parameter, cannot remove from list ");
        goto EXIT;
    }

    llist_node_t *p_current = p_llist->p_head;

    if (p_llist->p_head == p_llist->p_tail) 
    {
        return_val = llist_delete_first(p_llist);
        goto EXIT;
    }

    while (p_current->p_next != p_llist->p_tail) 
    {
        p_current = p_current->p_next;
    }

    node_destroy(p_llist->p_tail);
    p_current->p_next = NULL;
    p_llist->p_tail = p_current;
    p_llist->num_nodes --;
    return_val = 0;
    L_DBG( "Deleted node at llist end ");

EXIT:
    return return_val;
}

/**
 * @brief Function to remove the first node in a linked list.
 * 
 * @param p_llist pointer to the linked list
 * @return int returns -1 on failure, 0 on success
 */
int8_t llist_delete_first(llist_t *p_llist)
{
    int8_t return_val = -1;

    if ((NULL == p_llist) || (NULL == p_llist->p_head)) 
    {
        L_DBG("Bad parameter, cannot remove from list ");
        goto EXIT;
    }
    
    llist_node_t *p_to_del = p_llist->p_head;

    p_llist->p_head = p_llist->p_head->p_next;

    if (NULL == p_llist->p_head)
    {
        p_llist->p_tail = NULL; 
    }

    node_destroy(p_to_del);
    p_llist->num_nodes --;
    return_val = 0;
    L_DBG( "Deleted node at llist front ");

EXIT:
    return return_val;
}

/**
 * @brief Function to display the first node of a linked list. Uses a shared 
 * pointer with the caller (p_size_out) to share the size of the data. 
 * p_size_out's memory must be managed by the caller. 
 * 
 * @param p_llist pointer to the linked list
 * @param p_size_out pointer to share size of the data
 * @return NULL on failure, void * to data on success
 */
void *llist_peek_first(llist_t *p_llist, size_t *p_size_out)
{
    if (NULL == p_llist || (NULL == p_llist->p_head)) 
    {
        L_DBG("Bad parameter, cannot peek first ");
        return NULL;
    }

    if (NULL != p_size_out)
    {
        *p_size_out = p_llist->p_head->data_size;
    }

    L_DBG("Peeked first data ");
    return p_llist->p_head->p_data;
}

/**
 * @brief Function to display the last node of a linked list. Uses a shared 
 * pointer with the caller (p_size_out) to share the size of the data.  
 * p_size_out's memory must be managed by the caller. 
 * 
 * @param p_llist pointer to the linked list
 * @param p_size_out pointer to share size of the data
 * @return NULL on failure, void * to data on success
 */
void *llist_peek_last(llist_t *p_llist, size_t *p_size_out)
{
    if ((NULL == p_llist) || (NULL == p_llist->p_head)) 
    {
        L_DBG("Bad parameter, cannot show ");
        return NULL;
    }

    if (NULL != p_size_out)
    {
        *p_size_out = p_llist->p_tail->data_size;
    }

    L_DBG("Peeked last data ");
    return p_llist->p_tail->p_data;
}

/**
 * @brief Function to pop the first value from the llist and remove the node 
 * from memory. Uses a shared pointer with the caller (p_size_out) to share the 
 * size of the data. p_size_out's memory must be managed by the caller. 
 * 
 * @param p_llist pointer to the linked list
 * @param p_size_out pointer to shared data buffer
 * @return void* pointer to the data on success, NULL on failure
 */
void *llist_pop_first(llist_t *p_llist, size_t *p_size_out)
{
    if (NULL == p_llist || (NULL == p_llist->p_head)) 
    {
        L_DBG("Bad parameter, cannot pop first ");
        goto EXIT;
    }

    llist_node_t *p_pop_node = p_llist->p_head;
    void *p_data = p_pop_node->p_data;

    if (NULL != p_size_out)
    {
        *p_size_out = p_pop_node->data_size;
    }

    p_llist->p_head = p_pop_node->p_next;
    if (NULL == p_llist->p_head)
    {
        p_llist->p_tail = NULL;
    }

    free(p_pop_node);
    p_llist->num_nodes --;
    L_DBG("Popped first data ");
    return p_data;

EXIT:
    return NULL;
}

/**
 * @brief Function to pop the last value from the llist and remove the node from
 * memory. Uses a shared pointer with the caller (p_size_out) to share the 
 * size of the data. p_size_out's memory must be managed by the caller. 
 * 
 * @param p_llist pointer to the linked list
 * @param p_size_out pointer to shared data buffer
 * @return void* pointer to the data on success, NULL on failure
 */
void *llist_pop_last(llist_t *p_llist, size_t *p_size_out)
{
    if (NULL == p_llist || (NULL == p_llist->p_head)) 
    {
        L_DBG("Bad parameter, cannot pop last ");
        goto EXIT;
    }

    llist_node_t *p_current = p_llist->p_head;

    if (p_llist->p_head == p_llist->p_tail) 
    {
        llist_pop_first(p_llist, p_size_out);
        goto EXIT;
    }

    llist_node_t *p_pop_node = p_llist->p_tail;

    while (p_current->p_next != p_pop_node) 
    {
        p_current = p_current->p_next;
    }

    void *p_data = p_pop_node->p_data;
    p_current->p_next = NULL;
    p_llist->p_tail = p_current;

    if (NULL != p_size_out)
    {
        *p_size_out = p_pop_node->data_size;
    }

    free(p_pop_node);
    p_llist->num_nodes --;
    L_DBG("Popped last data ");
    return p_data;

EXIT:
    return NULL;
}

#if INCLUDE_SORT
/**
 * @brief Function to sort contents of a linked list by numerical value
 * ascending. Uses a bubble sort, and a do while loop. 
 * ATTENTION: Assumes contents are numeric (does not work for strings)
 * @param p_llist pointer to the linked list to sort
 * @return int8_t -1 on failure, 0 on success
 */
int8_t llist_num_sort(llist_t *p_llist)
{
    int8_t return_val = -1;
    int8_t sort = 0;

    if (NULL == p_llist)
    {
        L_DBG("Bad parameter, cannot sort");
        goto EXIT;
    }

    if (NULL == p_llist->p_head)
    {
        L_DBG("List is empty, nothing to sort ");
        return_val = 0;
        goto EXIT;
    }

    if (NULL == p_llist->p_head->p_next)
    {
        L_DBG("List is one node, nothing to sort ");
        return_val = 0;
        goto EXIT;
    }

    L_DBG("Sorting Contents (%d nodes) ", p_llist->num_nodes);

    do 
    {
        sort = 0;
        llist_node_t *p_current = p_llist->p_head;

        while (p_current->p_next != NULL)
        {
            // ATTENTION: Assumes data is int
            int val1 = *(int *)(p_current->p_data);
            int val2 = *(int *)(p_current->p_next->p_data);

            if (val1 > val2)
            {
                void *temp_data = p_current->p_data;
                p_current->p_data = p_current->p_next->p_data;
                p_current->p_next->p_data = temp_data;

                sort = 1;
            }
            p_current = p_current->p_next;
        }
    } while (sort);

    return_val = 0;

EXIT:
    return return_val;
}
#endif // INCLUDE_SORT

/**
 * @brief Function to add a new node at any index of a linked list.
 * 
 * @param p_llist pointer to the linked list
 * @param p_new_node pointer to the new node to add
 * @param index_in value to insert node at
 * @return int returns -1 on failure, 0 on success
 */
int8_t llist_add_index(llist_t *p_llist, llist_node_t *p_new_node, \
    int index_in)
{
    int8_t return_val = -1;

    if ((NULL == p_llist) || (NULL == p_new_node))
    {
        L_DBG("Bad parameter, cannot add to linked list ");
        goto EXIT;
    }
    
    int index = index_in;
    llist_node_t *p_current = p_llist->p_head;
    
    if (index < 0 || index > p_llist->num_nodes) 
    {
        L_DBG("Index outside of range ");
        goto EXIT;
    }

    if (index == 0)
        {
        return_val = llist_add_first(p_llist, p_new_node);
        goto EXIT;
        }

    if (index == p_llist->num_nodes)
        {
        return_val = llist_add_last(p_llist, p_new_node);
        goto EXIT;
        }

    for (int i = 0; i < index - 1; i ++)
    {
        p_current = p_current->p_next;
    }

    p_new_node->p_next = p_current->p_next;
    p_current->p_next = p_new_node;
    p_llist->num_nodes ++;
    return_val = 0;
    L_DBG( "Added node to llist index %d", index_in);

EXIT:
    return return_val;
}

#if INCLUDE_INDEX
/**
 * @brief Function to remove a node at any index in a linked list.
 * 
 * @param p_llist pointer to the linked list
 * @param index_in desired index to remove
 * @return int returns -1 on failure, 0 on success
 */
int8_t llist_delete_index(llist_t *p_llist, int index_in)
{
    int8_t return_val = -1;

    if ((NULL == p_llist) || (NULL == p_llist->p_head))
    {
        L_DBG("Bad parameter, unable to remove node ");
        goto EXIT;
    }

    int index = index_in;

    if (0 > index || p_llist->num_nodes <= index)
    {
        L_DBG("Index outside of range ");
        goto EXIT;
    }

    if (index == 0)
    {
        return_val = llist_delete_first(p_llist);
        goto EXIT;
    }

    if (index == (p_llist->num_nodes - 1))
    {
        return_val = llist_delete_last(p_llist);
        goto EXIT;
    }
    
    llist_node_t *p_prev = p_llist->p_head;

    for (int i = 0; i < (index - 1); i ++) 
    {
        p_prev = p_prev->p_next;
    }

    llist_node_t *p_to_del = p_prev->p_next;
    p_prev->p_next = p_to_del->p_next;

    node_destroy(p_to_del);
    p_llist->num_nodes --;
    return_val = 0;
    L_DBG( "Deleted node at index %d", index_in);

EXIT:
    return return_val;
}

/**
 * @brief Function to display the value of a selected index in a linked list. 
 * Uses a shared pointer with the caller (p_size_out) to share the size of the 
 * data. p_size_out's memory must be managed by the caller. 
 * 
 * @param p_llist pointer to the linked list
 * @param index_in index of node to peek
 * @param p_size_out pointer to share size of data
 * @return NULL on failure, void * to data on success
 */
void *llist_peek_index(llist_t *p_llist, int index_in, size_t *p_size_out)
{
    if (NULL == p_llist)
    {
        L_DBG("Bad parameter, cannot peek node");
        return NULL;
    }
    
    llist_node_t *p_current = p_llist->p_head;
    int index = index_in;

    if (0 > index || p_llist->num_nodes <= index)
    {
        L_DBG("Index outside of range ");
        return NULL;
    }

    if (0 == index) 
    {
        return llist_peek_first(p_llist, p_size_out);
    }

    if (index == (p_llist->num_nodes - 1)) 
    {
        return llist_peek_last(p_llist, p_size_out);
    }
    
    for (int i = 0; i < index; i ++ )
    {
        p_current = p_current->p_next;
    }
    
    L_DBG("Data found at index: %d ", index);
    
    if (NULL != p_size_out)
    {
        *p_size_out = p_current->data_size;
    }
            
    return p_current->p_data;
}

/**
 * @brief Function to pop the value at an index of llist and remove the node 
 * from memory. Uses a shared pointer with the caller (p_size_out) to share the 
 * size of the data. p_size_out's memory must be managed by the caller. 
 * 
 * @param index_in index of data to pop
 * @param p_llist pointer to the linked list
 * @param p_size_out pointer to shared data buffer
 * @return void* pointer to the data on success, NULL on failure
 */
void *llist_pop_index(llist_t *p_llist, int index_in, size_t *p_size_out)
{
    if (NULL == p_llist)
    {
        L_DBG("Bad parameter, cannot pop node");
        return NULL;
    }
    
    llist_node_t *p_current = p_llist->p_head;
    int index = index_in;

    if (0 > index || p_llist->num_nodes <= index)
    {
        L_DBG("Index outside of range ");
        return NULL;
    }

    if (0 == index) 
    {
        return llist_pop_first(p_llist, p_size_out);
    }

    if (index == (p_llist->num_nodes - 1)) 
    {
        return llist_pop_last(p_llist, p_size_out);
    }
    
    for (int i = 0; i < index - 1; i ++ )
    {
        p_current = p_current->p_next;
    }
    
    llist_node_t *p_pop_node = p_current->p_next;
    void *p_data = p_pop_node->p_data;
    p_current->p_next = p_pop_node->p_next;

    if (NULL != p_size_out)
    {
        *p_size_out = p_pop_node->data_size;
    }
    
    free(p_pop_node);
    p_llist->num_nodes --;
    return p_data;
}
#endif // INCLUDE_INDEXING

// End of file
