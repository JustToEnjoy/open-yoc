/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

// #include "pthread.h"
#include "pthread_internal.h"
#include <errno.h>

#if (POSIX_CONFIG_PTHREAD_ENABLE > 0)

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    int ret = -1;

    if (cond == NULL) {
        return -1;
    }

    ret = krhino_mutex_dyn_create(K_MUTEX_PP(&cond->lock), "mutex");
    ret |= krhino_sem_dyn_create(K_SEM_PP(&cond->wait_sem), "sem", 0);
    ret |= krhino_sem_dyn_create(K_SEM_PP(&cond->wait_done), "sem", 0);
    if (ret != 0) {
        return -1;
    }

    cond->waiting = cond->signals = 0;

    return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
    if (cond != NULL) {
        if (cond->wait_sem != NULL) {
            krhino_sem_dyn_del(cond->wait_sem);
        }

        if (cond->wait_done != NULL) {
            krhino_sem_dyn_del(cond->wait_done);
        }

        if (cond->lock != NULL) {
            krhino_mutex_dyn_del(cond->lock);
        }
    }

    return 0;
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
    int i           = 0;
    int num_waiting = 0;

    /* If there are waiting threads not already signalled, then
    signal the condition and wait for the thread to respond.
    */
    krhino_mutex_lock(cond->lock, RHINO_WAIT_FOREVER);

    if (cond->waiting > cond->signals) {
        num_waiting = (cond->waiting - cond->signals);
        cond->signals = cond->waiting;

        for (i = 0; i < num_waiting; ++i) {
            krhino_sem_give(cond->wait_sem);
        }
        /* Now all released threads are blocked here, waiting for us.
        Collect them all (and win fabulous prizes!) :-)
        */
        krhino_mutex_unlock(cond->lock);
        for (i = 0; i < num_waiting; ++i) {
            krhino_sem_take(cond->wait_done, RHINO_WAIT_FOREVER);
        }
    } else {
        krhino_mutex_unlock(cond->lock);
    }

    return 0;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
    /* If there are waiting threads not already signalled, then
    signal the condition and wait for the thread to respond.
    */
    krhino_mutex_lock(cond->lock, RHINO_WAIT_FOREVER);
    if ( cond->waiting > cond->signals ) {
        ++cond->signals;
        krhino_sem_give(cond->wait_sem);
        krhino_mutex_unlock(cond->lock);
        krhino_sem_take(cond->wait_done, RHINO_WAIT_FOREVER);
    } else {
        krhino_mutex_unlock(cond->lock);
    }

    return 0;
}

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
    kstat_t        retval;
    tick_t         ticks;
    struct timeval now;

    if (cond == NULL || mutex == NULL) {
        return EINVAL;
    }
    gettimeofday(&now, NULL);

    if (abstime == NULL) {
        ticks = MAX_TIMER_TICKS;
    } else {
        ticks = ((tick_t)(abstime->tv_sec - now.tv_sec)) * RHINO_CONFIG_TICKS_PER_SECOND +
                ((abstime->tv_nsec - now.tv_usec*1000) / 1000000) / (1000 / RHINO_CONFIG_TICKS_PER_SECOND);
    }

    /* Obtain the protection mutex, and increment the number of waiters.
       This allows the signal mechanism to only perform a signal if there
       are waiting threads.
     */
    krhino_mutex_lock(cond->lock, RHINO_WAIT_FOREVER);
    ++cond->waiting;
    krhino_mutex_unlock(cond->lock);

    /* Unlock the mutex, as is required by condition variable semantics */
    krhino_mutex_unlock(mutex->mutex);

    /* Wait for a signal */
    retval = krhino_sem_take(cond->wait_sem, ticks);

    /* Let the signaler know we have completed the wait, otherwise
           the signaler can race ahead and get the condition semaphore
           if we are stopped between the mutex unlock and semaphore wait,
           giving a deadlock.  See the following URL for details:
        http://www-classic.be.com/aboutbe/benewsletter/volume_III/Issue40.html
    */
    krhino_mutex_lock(cond->lock, RHINO_WAIT_FOREVER);

    if (cond->signals > 0) {
        /* If we timed out, we need to eat a condition signal */
        if (retval == RHINO_BLK_TIMEOUT) {
            krhino_sem_take(cond->wait_sem, RHINO_WAIT_FOREVER);
        }
        /* We always notify the signal thread that we are done */
        krhino_sem_give(cond->wait_done);

        /* Signal handshake complete */
        --cond->signals;
    }

    --cond->waiting;

    krhino_mutex_unlock(cond->lock);

    /* Lock the mutex, as is required by condition variable semantics */
    krhino_mutex_lock(mutex->mutex, RHINO_WAIT_FOREVER);

    if (retval == RHINO_BLK_TIMEOUT) {
        return ETIMEDOUT;
    }

    return 0;
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    kstat_t retval;

    /* Obtain the protection mutex, and increment the number of waiters.
       This allows the signal mechanism to only perform a signal if there
       are waiting threads.
     */
    krhino_mutex_lock(cond->lock, RHINO_WAIT_FOREVER);
    ++cond->waiting;
    krhino_mutex_unlock(cond->lock);

    /* Unlock the mutex, as is required by condition variable semantics */
    krhino_mutex_unlock(mutex->mutex);

    /* Wait for a signal */
    retval = krhino_sem_take(cond->wait_sem, RHINO_WAIT_FOREVER);

    /* Let the signaler know we have completed the wait, otherwise
           the signaler can race ahead and get the condition semaphore
           if we are stopped between the mutex unlock and semaphore wait,
           giving a deadlock.  See the following URL for details:
        http://www-classic.be.com/aboutbe/benewsletter/volume_III/Issue40.html
    */
    krhino_mutex_lock(cond->lock, RHINO_WAIT_FOREVER);

    if (cond->signals > 0) {
        /* If we timed out, we need to eat a condition signal */
        if (retval == RHINO_BLK_TIMEOUT) {
            krhino_sem_take(cond->wait_sem, RHINO_WAIT_FOREVER);
        }
        /* We always notify the signal thread that we are done */
        krhino_sem_give(cond->wait_done);

        /* Signal handshake complete */
        --cond->signals;
    }

    --cond->waiting;

    krhino_mutex_unlock(cond->lock);

    /* Lock the mutex, as is required by condition variable semantics */
    krhino_mutex_lock(mutex->mutex, RHINO_WAIT_FOREVER);

    return 0;
}

int pthread_condattr_init(pthread_condattr_t *attr)
{
    if (attr == NULL) {
        return -1;
    }

    attr->is_initialized = 1;

    attr->clock   = DEFAULT_COND_CLOCK;
    attr->pshared = DEFAULT_COND_SHARED;

    return 0;
}

int pthread_condattr_destroy(pthread_condattr_t *attr)
{
    if (attr == NULL) {
        return -1;
    }

    memset(attr, 0 ,sizeof(pthread_condattr_t));

    return 0;
}

#endif
