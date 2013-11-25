/**
 * @file    pause.h
 * @brief   
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

#ifndef _PAUSE_H
#define _PAUSE_H

#include <pthread.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/**
 * @brief Object that stores the state.
 */
typedef struct Pause_Obj {
    int             pause;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} Pause_Obj;

/**
 * @brief Pointer to a pause object
 */
typedef Pause_Obj *Pause_Handle;

/**
 * @brief Indicates that a Pause API call completed successfully.
 */
#define PAUSE_SUCCESS 0

/**
 * @brief Indicates that a Pause API call failed.
 */
#define PAUSE_FAILURE -1

/**
 * @brief Opens the Pause object. Must be called before other API:s on
 *        a Pause object.
 * @param hPause Pointer to the pause object to open.
 */
static inline void Pause_open(Pause_Handle hPause)
{
    pthread_mutex_init(&hPause->mutex, NULL);
    pthread_cond_init(&hPause->cond, NULL);

    hPause->pause = FALSE;
}

/**
 * @brief Called to see if processing is supposed to pause. If so, block the
 *        the execution of the thread. Otherwise continue.
 * @param hPause Pointer to a previously opened Pause object.
 */
static inline void Pause_test(Pause_Handle hPause)
{
    pthread_mutex_lock(&hPause->mutex);
    
    if (hPause->pause == TRUE) {
        pthread_cond_wait(&hPause->cond, &hPause->mutex);
    }

    pthread_mutex_unlock(&hPause->mutex);
}

/**
 * @brief Called to set processing threads to pause.
 * @param hPause Pointer to a previously opened Pause object.
 */
static inline void Pause_on(Pause_Handle hPause)
{
    pthread_mutex_lock(&hPause->mutex);

    hPause->pause = TRUE;

    pthread_mutex_unlock(&hPause->mutex);
}

/**
 * @brief Called to release processing threads currently paused.
 * @param hPause Pointer to a previously opened Pause object.
 */
static inline void Pause_off(Pause_Handle hPause)
{
    pthread_mutex_lock(&hPause->mutex);

    if (hPause->pause == TRUE) {
        hPause->pause = FALSE;
        pthread_cond_broadcast(&hPause->cond);
    }

    pthread_mutex_unlock(&hPause->mutex);
}

/**
 * @brief Closes a previously opened Pause object.
 * @param hPause Pointer to the pause object to close.
 */
static inline void Pause_close(Pause_Handle hPause)
{
    pthread_mutex_destroy(&hPause->mutex);
    pthread_cond_destroy(&hPause->cond);
}

#endif // _PAUSE_H
