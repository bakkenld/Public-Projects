/**
 * @file tpool.h
 * @author Logan Bakken
 * @brief Thread pool that uses a queue to assign work
 * @version 0.1
 * @date 2026-01-27
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef TPOOL_H
#define TPOOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include "logger.h"

typedef void (*thread_func_t)(void *p_arg);

typedef struct tpool_work {
    thread_func_t      func;
    void              *p_arg;
    struct tpool_work *p_next;
} tpool_work_t;

typedef struct tpool {
    tpool_work_t    *p_work_first;
    tpool_work_t    *p_work_last;
    pthread_mutex_t  work_mutex;
    pthread_cond_t   work_cond;
    pthread_cond_t   working_cond;
    size_t           thread_cnt;
    pthread_t       *p_all_threads;
    atomic_size_t    working_cnt;
    bool             finished;
} tpool_t;

// Life cycle operations:
tpool_t *tpool_create(size_t num);
void tpool_destroy(tpool_t *p_tm);

// Work processing:
bool tpool_add_work(tpool_t *p_tm, thread_func_t func, void *p_arg);
void tpool_wait(tpool_t *p_tm);

#endif // TPOOL_H

// End of file
