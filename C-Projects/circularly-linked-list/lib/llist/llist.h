/**
 * @file llist.h
 * @author Logan Bakken
 * @brief Funcitons for a singly linked list. Macro guards are used to trim out 
 * unused functions for various data structures. Set LLIST_FULL_BUILD to zero to 
 * exclude indexing/sort functions. Suitable for any data type. 
 * 
 * ATTENTION: Sorting is specifically excluded from this cllist. This is for 
 * simplicity sake, and not recognizing a good reason why a circular list should
 * be numerically sorted. 
 * 
 * @version 0.3
 * @date 2026-02-27
 * 
 * @copyright Copyright (c) 2026
 * 
 */


#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "logger.h"

// 1 to include all functions, 0 to exclude indexing and sorting operations.
// 1 is default 
#ifndef LLIST_FULL_BUILD
    #define LLIST_FULL_BUILD 1
#endif

// Feature Flags: Sorting is intentionally excluded from this build. Refer to 
// The above ATTENTION flag for more info. 
#if LLIST_FULL_BUILD 
    #define INCLUDE_INDEXING 1
    #define INCLUDE_SORT     0
#else
    #define INCLUDE_INDEXING 0
    #define INCLUDE_SORT     0
#endif

typedef struct llist_node_t
{
    void *p_data;
    size_t data_size;
    struct llist_node_t *p_next;
} llist_node_t;

typedef struct llist_t 
{
    int num_nodes;
    llist_node_t *p_head;
    llist_node_t *p_tail;
} llist_t;

// Life cycle functions: 
llist_node_t *node_create(void *p_data_in, size_t data_size_in);
int8_t node_destroy(llist_node_t *p_node_to_des); // helper delete function
llist_t *llist_create(void);
int8_t llist_clear(llist_t **pp_llist);
int8_t llist_destroy(llist_t **p_llist);

// Adding nodes:
int8_t llist_add_last(llist_t *p_llist, llist_node_t *p_new_node); 
int8_t llist_add_first(llist_t *p_llist, llist_node_t *p_new_node);

// Deleting nodes:
int8_t llist_delete_last(llist_t *p_llist);
int8_t llist_delete_first(llist_t *p_llist);

// Peeking at nodes:
void *llist_peek_first(llist_t *p_llist, size_t *p_size_out);
void *llist_peek_last(llist_t *p_llist, size_t *p_size_out);

// Popping nodes:
void *llist_pop_first(llist_t *p_llist, size_t *p_size_out);
void *llist_pop_last(llist_t *p_llist, size_t *p_size_out);

// Indexing and sorting:
int8_t llist_num_sort(llist_t *p_llist);
int8_t llist_add_index(llist_t *p_llist, llist_node_t *p_new_node, \
    int index_in);
int8_t llist_delete_index(llist_t *p_llist, int index_in);
void *llist_peek_index(llist_t *p_llist, int index_in, size_t *p_size_out);
void *llist_pop_index(llist_t *p_llist, int index_in, size_t *p_size_out);

#endif // LLIST_H

// end of file
