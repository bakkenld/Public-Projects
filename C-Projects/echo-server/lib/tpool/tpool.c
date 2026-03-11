/**
 * @file tpool.c
 * @author Logan Bakken
 * @brief Thread pool that uses a queue to assign work
 * @version 0.1
 * @date 2026-01-27
 * @copyright Copyright (c) 2026
 * 
 */

#include "tpool.h"

#define DEFAULT_THREADS 4 // Also defined in startup.h

static void *tpool_worker(void *p_arg);
static void tpool_work_destroy(tpool_work_t *p_work);
static tpool_work_t *tpool_work_create(thread_func_t func, \
    void *p_arg);
static tpool_work_t *tpool_work_get(tpool_t *p_tm);

/**
 * @brief Function to create the thread pool. Threads do not operate detached
 * so that shutdown/destruction can be synchronized. 
 * 
 * @param num number of threads to use. 4 is default
 * @return tpool_t* pointer to the thread pool
 */
tpool_t *tpool_create(size_t num)
{
    tpool_t   *p_tm = NULL;

    if (0 == num)
    {
        num = DEFAULT_THREADS;
    }

    /**
     * @brief Replace above "if" with this block for system defined thread count
     * #include <unistd.h>
     * num = 1 + (unsigned int)sysconf(_SC_NPROCESSORS_ONLN);
     */

    p_tm = calloc(1, sizeof(*p_tm));
    
    if (NULL == p_tm)
    {
        L_ERR("Failed to allocate memory to create thread pool");
        return NULL; 
    }

    p_tm->p_all_threads = calloc(num, sizeof(pthread_t));

    if (NULL == p_tm->p_all_threads)
    {
        L_ERR("Failed to allocate memory to create thread ID pool");
        free(p_tm);
        return NULL; 
    }

    p_tm->thread_cnt = num;
    p_tm->working_cnt = 0;

    pthread_mutex_init(&(p_tm->work_mutex), NULL);
    pthread_cond_init(&(p_tm->work_cond), NULL);
    pthread_cond_init(&(p_tm->working_cond), NULL);

    p_tm->p_work_first = NULL;
    p_tm->p_work_last  = NULL;

    for (size_t i = 0; i < num; i ++) 
    {
        if (0 != pthread_create(&(p_tm->p_all_threads[i]), NULL, tpool_worker, \
        p_tm)) 
        {
            L_ERR("Failed to create worker thread");
            free(p_tm->p_all_threads);
            free(p_tm);
            return NULL;
        }
    }
    L_SUCCESS("Created Thread Pool");
    return p_tm;
}

/**
 * @brief Function to destroy the thread pool. First, any remaining work is 
 * removed from the work queue, then workers are signaled to see there is no 
 * work and join for destruction. 
 * 
 * @param p_tm pointer to the thread pool to destroy
 */
void tpool_destroy(tpool_t *p_tm)
{
    tpool_work_t *p_work = NULL;
    tpool_work_t *p_work2 = NULL;

    if (NULL == p_tm)
    {
        L_ERR("_destroy NULL parameter: p_tm");
        return;
    }

    pthread_mutex_lock(&(p_tm->work_mutex));
    p_work = p_tm->p_work_first;
    
    while (p_work != NULL) 
    {
        p_work2 = p_work->p_next;
        tpool_work_destroy(p_work);
        p_work = p_work2;
    }
    
    p_tm->p_work_first = NULL;
    p_tm->finished = true;
    
    pthread_cond_broadcast(&(p_tm->work_cond));
    pthread_mutex_unlock(&(p_tm->work_mutex));

    for (size_t i = 0; i < p_tm->thread_cnt; i++) 
    {
        pthread_join(p_tm->p_all_threads[i], NULL);
    }

    pthread_mutex_destroy(&(p_tm->work_mutex));
    pthread_cond_destroy(&(p_tm->work_cond));
    pthread_cond_destroy(&(p_tm->working_cond));

    free(p_tm->p_all_threads);
    free(p_tm);
    L_SUCCESS("Destroyed Thread Pool");
}

/**
 * @brief Function to create work for the thread pool
 * 
 * @param func function pointer to the "work" function
 * @param p_arg the data the "work" function needs to run
 * @return tpool_work_t* pointer to the work, or NULL on failure
 */
static tpool_work_t *tpool_work_create(thread_func_t func, void *p_arg)
{
    tpool_work_t *p_work = NULL;

    if (NULL == func )
    {
        L_ERR("_work_create NULL parameter: p_work");
        return NULL;
    }

    p_work = calloc(1, sizeof(*p_work));

    if (NULL == p_work)
    {
        L_ERR("Failed to allocate memory to create work");
        return NULL;
    }

    p_work->func = func;
    p_work->p_arg  = p_arg;
    p_work->p_next = NULL;
    L_DBG("Work created");
    return p_work;
}

/**
 * @brief Function to destroy a job
 * 
 * @param work pointer to the work to free
 */
static void tpool_work_destroy(tpool_work_t *p_work)
{
    if (NULL == p_work)
    {
        L_ERR("_work_destroy NULL parameter: p_work");
        return;
    }

    free(p_work);
    p_work = NULL;
    L_DBG("Work destroyed");
}

/**
 * @brief Function to add work to the working queue. The working queue functions
 * LIFO
 * 
 * @param p_tm pointer to the thread pool
 * @param func the "work" function
 * @param p_arg data for the "work" function
 * @return true 
 * @return false 
 */
bool tpool_add_work(tpool_t *p_tm, thread_func_t func, void *p_arg)
{
    tpool_work_t *p_work = NULL;
    tpool_work_t **pp_work_active = NULL;

    if (NULL == p_tm)
    {
        L_ERR("_add_work NULL parameter: p_tm");
        return false;
    }    

    p_work = tpool_work_create(func, p_arg);
    
    if (NULL == p_work)
    {
        L_ERR("_add_work NULL parameter: p_work");
        return false; 
    }

    pthread_mutex_lock(&(p_tm->work_mutex));    
    
    pp_work_active = &(p_tm->p_work_first);
    p_work->p_next = *pp_work_active;
    *pp_work_active = p_work;

    if (NULL == p_work->p_next) 
    {
        p_tm->p_work_last = p_work;
    }

    pthread_cond_broadcast(&(p_tm->work_cond));
    pthread_mutex_unlock(&(p_tm->work_mutex));

    L_DBG("Work added");
    return true;
}


/**
 * @brief Function to get work from the queue
 * 
 * @param p_tm pointer to the pool
 * @return tpool_work_t* pointer to the work
 */
static tpool_work_t *tpool_work_get(tpool_t *p_tm)
{
    tpool_work_t *p_work = NULL;

    if (NULL == p_tm)
    {
        L_ERR("_work_get NULL parameter: p_tm");
        return NULL;
    }

    p_work = p_tm->p_work_first;
    
    if (NULL == p_work)
    {
        return NULL; 
    }

    if (NULL == p_work->p_next) 
    {
        p_tm->p_work_first = NULL;
        p_tm->p_work_last  = NULL;
    } 
    
    else 
    {
        p_tm->p_work_first = p_work->p_next;
    }

    L_DBG("Retrieved work");
    return p_work;
}

/**
 * @brief Function to define a worker thread. Worker threads block until work is
 * signaled. When work is signaled, they lock, increment the active 
 * counter, extract the work, and unlock. When a task completes, 
 * the worker decrements the working count and signals if the pool is empty. 
 * 
 * @param p_arg the data for the "work" function
 * @return void* 
 */
static void *tpool_worker(void *p_arg)
{
    tpool_t      *p_tm = p_arg;
    tpool_work_t *p_work;

    while (1) 
    {
        pthread_mutex_lock(&(p_tm->work_mutex));

        while (NULL == p_tm->p_work_first && !p_tm->finished)
        {
            pthread_cond_wait(&(p_tm->work_cond), &(p_tm->work_mutex));
        }
            
        if (p_tm->finished && NULL == p_tm->p_work_first)
        {
            pthread_mutex_unlock(&(p_tm->work_mutex));
            break; 
        }

        p_work = tpool_work_get(p_tm);

        p_tm->working_cnt ++;

        pthread_mutex_unlock(&(p_tm->work_mutex));

        if (NULL != p_work) 
        {
            p_work->func(p_work->p_arg);
            tpool_work_destroy(p_work);
        }

        p_tm->working_cnt --;

        if (!p_tm->finished && 0 == p_tm->working_cnt  && \
            NULL == p_tm->p_work_first)
        {
            pthread_cond_signal(&(p_tm->working_cond));
        }
    }

    return NULL;
}

// End of file
