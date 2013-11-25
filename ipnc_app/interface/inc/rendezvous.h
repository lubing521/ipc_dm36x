/**
 * @file    rendezvous.h
 * @brief   
 *
 * This interface enables multiple threads to do their initialization in
 * paralell before meeting up and start the main processing loops.
 *
 * @verbatim
 * ============================================================================
 * Copyright (c) Texas Instruments Inc 2007
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied or provided.
 * ============================================================================
 * @endverbatim
 */

#ifndef _RENDEZVOUS_H
#define _RENDEZVOUS_H

#include <pthread.h>

/**
 * @brief Object that stores the state.
 */
typedef struct Rendezvous_Obj {
    int             orig;
    int             count;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} Rendezvous_Obj;

/**
 * @brief Pointer to a rendezvous object
 */
typedef Rendezvous_Obj *Rendezvous_Handle;

/**
 * @brief Indicates that a Rendezvous API call completed successfully.
 */
#define RENDEZVOUS_SUCCESS 0

/**
 * @brief Indicates that a Rendezvous API call failed.
 */
#define RENDEZVOUS_FAILURE -1

/**
 * @brief Opens the Rendezvous object. Must be called before other API:s on
 * a Rendezvous object.
 * @param hRv Pointer to the rendezvous object to open.
 * @param count Number of threads to synchronize initialization for.
 */
static inline void Rendezvous_open(Rendezvous_Handle hRv, int count)
{
    pthread_mutex_init(&hRv->mutex, NULL);
    pthread_cond_init(&hRv->cond, NULL);

    hRv->count = count;
    hRv->orig = count;
}

/**
 * @brief Called by a thread when it's initialization is done. This will
 * register that this thread is done initializing, and will block the calling
 * thread until the other threads are done initializing, after which all
 * threads will be unblocked at once.
 * @param hRv Pointer to a previously opened Rendezvous object.
 * @return RENDEZVOUS_SUCCESS for success or RENDEZVOUS_FAILURE for failure.
 */
static inline void Rendezvous_meet(Rendezvous_Handle hRv)
{
    pthread_mutex_lock(&hRv->mutex);
    hRv->count--;

    if (hRv->count > 0) {
        pthread_cond_wait(&hRv->cond, &hRv->mutex);
    } 
    else {
        pthread_cond_broadcast(&hRv->cond);
    }

    pthread_mutex_unlock(&hRv->mutex);
}

/**
 * @brief This call forces all threads blocking in Rendezvous_meet to unblock
 * no matter what the state of the Rendezvous object. Useful for error cleanup.
 * @param hRv Pointer to a previously opened Rendezvous object.
 */
static inline void Rendezvous_force(Rendezvous_Handle hRv)
{
    pthread_mutex_lock(&hRv->mutex);
    hRv->count = 0;
    pthread_cond_broadcast(&hRv->cond);
    pthread_mutex_unlock(&hRv->mutex);
}

/**
 * @brief This call resets the Rendezvous object to it's original count at
 *        opening time.
 * @param hRv Pointer to a previously opened Rendezvous object.
 */
static inline void Rendezvous_reset(Rendezvous_Handle hRv)
{
    pthread_mutex_lock(&hRv->mutex);
    hRv->count = hRv->orig;
    pthread_mutex_unlock(&hRv->mutex);
}

/**
 * @brief Closes a previously opened Rendezvous object.
 * @param hRv Pointer to the rendezvous object to close.
 */
static inline void Rendezvous_close(Rendezvous_Handle hRv)
{
    pthread_mutex_destroy(&hRv->mutex);
    pthread_cond_destroy(&hRv->cond);
}

#endif // _RENDEZVOUS_H
