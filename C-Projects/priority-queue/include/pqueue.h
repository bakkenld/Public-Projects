/**
 * @file pqueue.h
 * @author Logan Bakken
 * @brief Functions for a priority queue. 
 * Capable of operating FIFO. Built on a linked list library
 * @version 0.3
 * @date 2026-02-27
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef pqueue_H
#define pqueue_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "llist.h"

typedef struct pqueue_item_t
{
    int8_t priority;   // Lower value = Higher priority (usually)
    void *p_data;
    size_t data_size;
} pqueue_item_t;

typedef struct llist_node_t pqueue_node_t;

typedef llist_t pqueue_t;

// Life cycle functions:
pqueue_t *pqueue_create(void);
int8_t pqueue_clear(pqueue_t **pp_pqueue);
int8_t pqueue_destroy(pqueue_t **pp_pqueue);

// Adding nodes:
int8_t pqueue_push(pqueue_t *p_pqueue, void *p_data, size_t data_size_in, \
    int8_t priority_in);

// Removing nodes:
void *pqueue_peek(pqueue_t *p_pqueue, size_t *p_size_out);
void *pqueue_pop(pqueue_t *p_pqueue, size_t *p_size_out);

#endif // pqueue_H

// end of file
