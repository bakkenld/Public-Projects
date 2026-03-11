/**
 * @file squeue.h
 * @author Logan Bakken
 * @brief Functions for a standard queue (stack). 
 * Capable of operating FIFO. Built on a linked list library
 * @version 0.3
 * @date 2026-02-25
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef SQUEUE_H
#define SQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "llist.h"

typedef llist_node_t squeue_node_t;
typedef llist_t squeue_t;

// Life cycle functions:
squeue_t *squeue_create(void);
squeue_node_t *node_create(void *p_data_in, size_t data_size_in);
int8_t squeue_clear(squeue_t **pp_squeue);
int8_t squeue_destroy(squeue_t **pp_squeue);

// Adding nodes:
int8_t squeue_push(squeue_t *p_squeue, squeue_node_t *p_new_node);

// Removing nodes:
void *squeue_peek(squeue_t *p_squeue, size_t *p_size_out);
void *squeue_pop(squeue_t *p_squeue, size_t *p_size_out);

#endif // SQUEUE_H

// end of file
