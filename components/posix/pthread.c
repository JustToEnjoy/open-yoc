/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

// #include "pthread.h"
#include "pthread_internal.h"
#include "timer.h"
#include <errno.h>

#if (POSIX_CONFIG_PTHREAD_ENABLE > 0)

kmutex_t g_pthread_mutex;

int pthread_lock_init(void)
{
    return krhino_mutex_create(&g_pthread_mutex, "g_pthread_mutex");
}

int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    /* fork is not supported, so pthread_atfork is not supported too */
    return -1;
}

void pthread_cleanup_pop(int execute)
{
    _pthread_tcb_t     *ptcb;
    _pthread_cleanup_t *cleanup;

    ptcb = _pthread_get_tcb(krhino_cur_task_get());

    if (ptcb && execute > 0) {
        cleanup = ptcb->cleanup;
        if (cleanup != NULL) {
            ptcb->cleanup = cleanup->prev;
        }

        if (cleanup != NULL) {
            cleanup->cleanup_routine(cleanup->para);
            krhino_mm_free(cleanup);
        }
    }
}

void pthread_cleanup_push(void (*routine)(void *), void *arg)
{
    _pthread_tcb_t     *ptcb;
    _pthread_cleanup_t *cleanup;

    ptcb = _pthread_get_tcb(krhino_cur_task_get());
    if (ptcb) {
        cleanup = (_pthread_cleanup_t *)krhino_mm_alloc(sizeof(_pthread_cleanup_t));
        if (cleanup != NULL) {
            cleanup->cleanup_routine = routine;
            cleanup->para            = arg;
            cleanup->prev            = ptcb->cleanup;

            ptcb->cleanup = cleanup;
            _alloc_trace(cleanup, (size_t)__builtin_return_address(0));
        }
    }
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg)
{
    kstat_t ret    = 0;
    void    *stack = NULL;

    _pthread_tcb_t *ptcb = NULL;
    _pthread_tcb_t *ptcb_c = NULL;

    if (thread == NULL) {
        return -1;
    }

    /* create _pthread_tcb_t */
    ptcb = (_pthread_tcb_t *)krhino_mm_alloc(sizeof(_pthread_tcb_t));
    if (ptcb == NULL) {
        return -1;
    }

    _alloc_trace(ptcb, (size_t)__builtin_return_address(0));
    memset(ptcb, 0, sizeof(_pthread_tcb_t));

    ptcb->canceled     = 0;
    ptcb->cancel_state = PTHREAD_CANCEL_DISABLE;
    ptcb->cancel_type  = PTHREAD_CANCEL_DEFERRED;
    ptcb->magic        = PTHREAD_MAGIC;
    strcpy(ptcb->name, "pthread");

    /* get pthread attr */
    if (attr != NULL) {
        if (attr->inheritsched == PTHREAD_INHERIT_SCHED) {
            ptcb_c = _pthread_get_tcb(krhino_cur_task_get());
            if (ptcb_c != NULL) {
                ptcb->attr = ptcb_c->attr;
            } else {
                ptcb->attr = *attr;
            }
        } else {
            ptcb->attr = *attr;
        }
    } else {
        ret = pthread_attr_init(&ptcb->attr);
        if (ret != 0) {
            return -1;
        }
    }

    /* get stack addr */
    if (ptcb->attr.stackaddr == 0) {
        stack = (void *)krhino_mm_alloc(ptcb->attr.stacksize);
        if (stack == NULL) {
            krhino_mm_free(ptcb);
            return -1;
        }
        _alloc_trace(stack, (size_t)__builtin_return_address(0));
    } else {
        stack = (void *)(ptcb->attr.stackaddr);
    }

    /* create ktask_t */
    ptcb->tid = krhino_mm_alloc(sizeof(ktask_t));
    if (ptcb->tid == NULL) {
        if (ptcb->attr.stackaddr == 0) {
            krhino_mm_free(stack);
        }

        krhino_mm_free(ptcb);
        return -1;
    }
    _alloc_trace(ptcb->tid, (size_t)__builtin_return_address(0));

    if (ptcb->attr.detachstate == PTHREAD_CREATE_JOINABLE) {
        ret = krhino_sem_dyn_create(&ptcb->join_sem, "join_sem", 0);
        if (ret != RHINO_SUCCESS) {
            if (ptcb->attr.stackaddr == NULL) {
                krhino_mm_free(stack);
            }

            krhino_mm_free(ptcb);
            return -1;
        }
    } else {
        ptcb->join_sem = 0;
    }

    ptcb->thread_entry = start_routine;
    ptcb->thread_para  = arg;

    ret = krhino_task_create(ptcb->tid, ptcb->name, arg,
                             ptcb->attr.schedparam.sched_priority, 0, stack,
                             (ptcb->attr.stacksize / sizeof(cpu_stack_t)),
                             (task_entry_t)start_routine, 0);

    if (ret != RHINO_SUCCESS) {
        if (ptcb->attr.stackaddr == NULL) {
            krhino_mm_free(stack);
        }

        if (ptcb->join_sem != NULL) {
            krhino_sem_dyn_del(ptcb->join_sem);
        }
        krhino_mm_free(ptcb);
        return -1;
    }

    *thread = ptcb->tid;
    ptcb->tid->user_info[PTHREAD_CONFIG_USER_INFO_POS] = ptcb;

    ret = krhino_task_resume(ptcb->tid);
    if (ret == RHINO_SUCCESS) {
        return RHINO_SUCCESS;
    }

    if (ptcb->attr.stackaddr == NULL) {
        krhino_mm_free(stack);
    }

    krhino_mm_free(ptcb);

    return -1;
}

void pthread_exit(void *value_ptr)
{
    //int ret = -1;

    _pthread_tcb_t *ptcb;

    ptcb = _pthread_get_tcb(krhino_cur_task_get());

    if (ptcb != NULL) {
        ptcb->return_value = value_ptr;
        /* ptcb->join_sem semaphore should be released by krhino_task_del_hook
         * if ptcb->attr.detachstate is PTHREAD_CREATE_DETACHED, task stack and
         * task structure should be release by krhino_task_del_hook
         */
        krhino_task_del(krhino_cur_task_get());
    }
}

int pthread_detach(pthread_t thread)
{
    int ret = -1;

    _pthread_tcb_t *ptcb;

    if (thread == NULL) {
        return -1;
    }

    if (thread == krhino_cur_task_get()) {
        return -1;
    }

    ptcb = _pthread_get_tcb(thread);

    if (ptcb == NULL) {
        return -1;
    }

    if (K_PTHREAD(thread)->task_state != K_DELETED) {
        if (ptcb->attr.detachstate == PTHREAD_CREATE_JOINABLE) {

            ret = krhino_mutex_lock(&g_pthread_mutex, RHINO_WAIT_FOREVER);
            if (ret != 0) {
                return -1;
            }

            ptcb->attr.detachstate = PTHREAD_CREATE_DETACHED;
            krhino_mutex_unlock(&g_pthread_mutex);

            if (ptcb->join_sem != NULL) {
                krhino_sem_give(ptcb->join_sem);
            }
        }
    } else {
        if (ptcb->attr.stackaddr == NULL) {
            krhino_mm_free(ptcb->tid->task_stack_base);
        }

        if (ptcb->join_sem != NULL) {
            ret = krhino_sem_dyn_del(ptcb->join_sem);
        }

        krhino_mm_free(ptcb->tid);
        krhino_mm_free(ptcb);
    }

    if (ret != 0) {
        return -1;
    }

    return 0;
}

int pthread_join(pthread_t thread, void **retval)
{
    kstat_t         ret = 0;
    _pthread_tcb_t *ptcb;

    if (thread == NULL) {
        return -1;
    }

    if (thread == krhino_cur_task_get()) {
        return -1;
    }

    ptcb = _pthread_get_tcb(thread);

    if (ptcb == NULL) {
        return -1;
    }

    if (ptcb->attr.detachstate == PTHREAD_CREATE_DETACHED) {
        return -1;
    }

    ret = krhino_sem_take(ptcb->join_sem, RHINO_WAIT_FOREVER);
    if (ret == RHINO_SUCCESS) {
        if (retval != 0) {
            *retval = ptcb->return_value;
        }

        krhino_sem_dyn_del(ptcb->join_sem);

        if (ptcb->attr.detachstate == PTHREAD_CREATE_JOINABLE) {
            if (ptcb->attr.stackaddr == 0) {
                krhino_mm_free(ptcb->tid->task_stack_base);
            }

            krhino_mm_free(ptcb->tid);
            krhino_mm_free(ptcb);
        }
    } else {
        return -1;
    }

    return 0;
}

int pthread_cancel(pthread_t thread)
{
    return -1;
}

void pthread_testcancel(void)
{
    return;
}

int pthread_setcancelstate(int state, int *oldstate)
{
    return -1;
}

int pthread_setcanceltype(int type, int *oldtype)
{
    return -1;
}

int pthread_kill(pthread_t thread, int sig)
{
    /* This api should not be used, and will not be supported */
    return -1;
}

int pthread_equal(pthread_t t1, pthread_t t2)
{
    return (int)(t1 == t2);
}

int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param)
{
    kstat_t ret     = -1;
    uint8_t old_pri = 0;

    _pthread_tcb_t *ptcb = NULL;

    if (param == NULL) {
        return -1;
    }

    if (policy == SCHED_FIFO) {
        ret = krhino_sched_policy_set((ktask_t *)thread, KSCHED_FIFO);
    } else if (policy == SCHED_RR) {
        ret = krhino_sched_policy_set((ktask_t *)thread, KSCHED_RR);
        if (ret == 0) {
            ret = krhino_task_time_slice_set((ktask_t *)thread, (param->slice * 1000) / RHINO_CONFIG_TICKS_PER_SECOND);
        }
    } else {
        return -1;
    }

    if (ret != RHINO_SUCCESS) {
        return -1;
    }

    /* change the priority of pthread */
    ret = krhino_task_pri_change((ktask_t *)thread, PRI_CONVERT_PX_RH(param->sched_priority), &old_pri);
    if (ret != RHINO_SUCCESS) {
        return -1;
    }

    ptcb = _pthread_get_tcb(thread);
    if (ptcb == NULL) {
        return -1;
    }

    ptcb->attr.schedparam = *param;

    return 0;
}

pthread_t pthread_self(void)
{
    return krhino_cur_task_get();
}

int pthread_setschedprio(pthread_t thread, int prio)
{
    uint8_t old_pri = 0;
    int     ret     = -1;

    if (thread == NULL) {
        return -1;
    }

    ret = krhino_task_pri_change((ktask_t *)thread, PRI_CONVERT_PX_RH(prio), &old_pri);
    if (ret != RHINO_SUCCESS) {
        return -1;
    }

    return 0;
}

int pthread_once(pthread_once_t *once_control, void (*init_routine)(void))
{
    int ret = -1;

    ret = krhino_mutex_lock(&g_pthread_mutex, RHINO_WAIT_FOREVER);
    if (ret != 0) {
        return -1;
    }

    if (*once_control == PTHREAD_ONCE_INIT) {
        *once_control = !PTHREAD_ONCE_INIT;

        krhino_mutex_unlock(&g_pthread_mutex);

        init_routine ();

        return 0;
    }

    krhino_mutex_unlock(&g_pthread_mutex);

    return 0;
}

int pthread_getcpuclockid(pthread_t thread_id, clockid_t *clock_id)
{
    if ((thread_id == NULL)||(clock_id == NULL)) {
        return -1;
    }

    *clock_id = CLOCK_MONOTONIC;

    return 0;
}

int pthread_getconcurrency(void)
{
    /* User thread and kernel thread are one-to-one correspondence in AliOS Things,
       so the concurrency is 0  */
    return 0;
}

int pthread_setconcurrency(int new_level)
{
    /* User thread and kernel thread are one-to-one correspondence in AliOS Things,
       so the concurrency can not be set  */
    return -1;
}

int pthread_setname_np(pthread_t thread, const char *name)
{
    _pthread_tcb_t *ptcb;

    if (thread == NULL) {
        return -1;
    }

    ptcb = _pthread_get_tcb(thread);

    if (ptcb == NULL) {
        return -1;
    }

    if (strlen(name) > DEFAULT_THREAD_NAME_LEN - 1) {
        return ERANGE;
    }

    strcpy(ptcb->name, name);

    return 0;
}

int pthread_getname_np(pthread_t thread, char *name, size_t len)
{
    _pthread_tcb_t *ptcb;

    if (thread == NULL) {
        return -1;
    }

    ptcb = _pthread_get_tcb(thread);

    if (ptcb == NULL) {
        return -1;
    }

    if (strlen(ptcb->name) > len - 1) {
        return ERANGE;
    }

    strcpy(name, ptcb->name);

    return 0;
}

#endif
