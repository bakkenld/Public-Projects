/**
 * @file cllist.h
 * @author Logan Bakken
 * @brief Funcitons for a circularly linked list
 * Suitable for ints and strs
 * In order to support any data type, this ccllist 
 * requires user defined print and data clearing functions. 
 * @version 0.2
 * @date 2026-02-27
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef CLLIST_H
#define CLLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "llist.h"

typedef struct llist_node_t cllist_node_t;

typedef struct llist_t cllist_t;

// Life cycle functions: 
cllist_t *cllist_create(void);
int8_t cllist_clear(cllist_t *p_cllist);
int8_t cllist_destroy(cllist_t **p_cllist);

// Adding nodes:
int8_t cllist_add_first(cllist_t *p_cllist, cllist_node_t *p_new_node);
int8_t cllist_add_last(cllist_t *p_cllist, cllist_node_t *p_new_node); 

// Deleting nodes:
int8_t cllist_delete_first(cllist_t *p_cllist);
int8_t cllist_delete_last(cllist_t *p_cllist);

// Peeking at nodes:
void *cllist_peek_first(cllist_t *p_cllist, size_t *p_size_out);
void *cllist_peek_last(cllist_t *p_cllist, size_t *p_size_out);

// Popping nodes:
void *cllist_pop_first(cllist_t *p_cllist, size_t *p_size_out);
void *cllist_pop_last(cllist_t *p_cllist, size_t *p_size_out);

// Indexing:
int8_t cllist_add_index(cllist_t *p_cllist, cllist_node_t *p_new_node, \
    int index_in);
int8_t cllist_delete_index(cllist_t *p_cllist, int index_in);
void *cllist_peek_index(cllist_t *p_cllist, int index_in, size_t *p_size_out);
void *cllist_pop_index(cllist_t *p_cllist, int index_in, size_t *p_size_out);

#endif // CLLIST_H

// end of file
