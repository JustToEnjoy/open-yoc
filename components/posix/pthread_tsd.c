/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

// #include "pthread.h"
#include "pthread_internal.h"
#include "posix_config.h"

#if (POSIX_CONFIG_PTHREAD_ENABLE > 0)

pthread_key_list_t pthread_key_list_head;

int pthread_key_create(pthread_key_t *key, void (*destructor)(void*))
{
    pthread_key_list_t *pthread_key_list_s_c = NULL;
    pthread_key_list_t *pthread_key_list_s_l = NULL;

    if (key == NULL) {
        return -1;
    }

    CPSR_ALLOC();
    RHINO_CRITICAL_ENTER();

    /* if the key list is empty int pthread_key_list_head to save the first key */
    if (pthread_key_list_head.key_num == 0) {
        pthread_key_list_head.key_num = 1;
        pthread_key_list_head.head.fun = destructor;
        *key = pthread_key_list_head.key_num;
    } else {
        /* if the key list is not empty find the last key and add key */
        /* find the last key */
        pthread_key_list_s_c = pthread_key_list_head.next;
        pthread_key_list_s_l = &pthread_key_list_head;
        while (pthread_key_list_s_c != NULL) {
            pthread_key_list_s_l = pthread_key_list_s_c;
            pthread_key_list_s_c = pthread_key_list_s_c->next;
        }

        /* malloc the new key */
        pthread_key_list_s_c = (pthread_key_list_t *)krhino_mm_alloc(sizeof(pthread_key_list_t));
        if (pthread_key_list_s_c == NULL) {
            RHINO_CRITICAL_EXIT();
            return -1;
        }
        _alloc_trace(pthread_key_list_s_c, (size_t)__builtin_return_address(0));

        /* init the new key */
        memset(pthread_key_list_s_c, 0, sizeof(pthread_key_list_t));

        /* insert the key to the end of list */
        pthread_key_list_s_c->key_num = pthread_key_list_s_l->key_num + 1;
        pthread_key_list_s_c->head.fun = destructor;
        pthread_key_list_s_l->next = pthread_key_list_s_c;

        /* update the key value */
        *key = pthread_key_list_s_c->key_num;
    }

    RHINO_CRITICAL_EXIT();

    return 0;
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
    pthread_key_list_t  *pthread_key_list_s_c = NULL;
    pthread_key_value_t *key_value_s_o        = NULL;
    pthread_key_value_t *key_value_s          = NULL;
    pthread_key_value_t *key_value_s_c        = NULL;
    pthread_key_value_t *key_value_s_l        = NULL;

    int list_flag  = 0;
    int value_flag = 0;

    if (value == NULL) {
        return -1;
    }

    CPSR_ALLOC();
    RHINO_CRITICAL_ENTER();

    /* find the key in list */
    pthread_key_list_s_c = &pthread_key_list_head;
    while (pthread_key_list_s_c != NULL) {
        if (pthread_key_list_s_c->key_num == key) {
            list_flag = 1;
            key_value_s_o = pthread_key_list_s_c->head.next;
            break;
        }

        pthread_key_list_s_c = pthread_key_list_s_c->next;
    }

    /* if can not find the key in list, return error */
    if (list_flag == 0) {
        RHINO_CRITICAL_EXIT();
        return -1;
    }

    /* if no value store in the key, create new pthread_key_value_t to save the value */
    if (key_value_s_o == NULL) {
        key_value_s = (pthread_key_value_t *)krhino_mm_alloc(sizeof(pthread_key_value_t));
        if (key_value_s == NULL) {
            RHINO_CRITICAL_EXIT();
            return -1;
        }

        _alloc_trace(key_value_s, (size_t)__builtin_return_address(0));
        memset(key_value_s, 0, sizeof(pthread_key_value_t));

        /* save the thread id and value */
        key_value_s->key_value.value = value;
        key_value_s->key_value.thread = krhino_cur_task_get();
        key_value_s->next = NULL;

        /* and pthread_key_value_t to value list */
        pthread_key_list_s_c->head.next = key_value_s;
    } else {
        /* if value store in the key, find the last value and add the new value */
        /* sreach the value list to find if same thread had save the value */
        key_value_s_c = key_value_s_o;
        while (key_value_s_c != NULL) {
            /* if the same thread had save the value update the value */
            if (key_value_s_c->key_value.thread == krhino_cur_task_get()) {
                key_value_s_c->key_value.value = value;
                value_flag = 1;

                break;
            }

            key_value_s_l = key_value_s_c;
            key_value_s_c = key_value_s_c->next;
        }

        /* if no same thread had save the value before create new pthread_key_value_t */
        if (value_flag == 0) {
            key_value_s = (pthread_key_value_t *)krhino_mm_alloc(sizeof(pthread_key_value_t));
            if (key_value_s == NULL) {
                RHINO_CRITICAL_EXIT();
                return -1;
            }

            _alloc_trace(key_value_s, (size_t)__builtin_return_address(0));
            memset(key_value_s, 0, sizeof(pthread_key_value_t));

            /* save current value to pthread_key_value_t */
            key_value_s->next = key_value_s_l->next;
            key_value_s->key_value.value = value;
            key_value_s->key_value.thread = krhino_cur_task_get();

            /* add the value to the list */
            key_value_s_l->next = key_value_s;
        }
    }

    RHINO_CRITICAL_EXIT();

    return 0;
}

void *pthread_getspecific(pthread_key_t key)
{
    pthread_key_list_t  *pthread_key_list_s_c = NULL;
    pthread_key_value_t *key_value_s_o        = NULL;
    pthread_key_value_t *key_value_s_c        = NULL;

    int list_flag = 0;

    CPSR_ALLOC();
    RHINO_CRITICAL_ENTER();

    /* find the key in list */
    pthread_key_list_s_c = &pthread_key_list_head;
    while (pthread_key_list_s_c != NULL) {
        if (pthread_key_list_s_c->key_num == key) {
            list_flag = 1;
            key_value_s_o = pthread_key_list_s_c->head.next;
            break;
        }

        pthread_key_list_s_c = pthread_key_list_s_c->next;
    }

    /* if can not find the key in list, or no value store in the key, return NULL */
    if ((list_flag == 0) || (key_value_s_o == NULL)) {
        RHINO_CRITICAL_EXIT();
        return NULL;
    }

    /* search the value list to find the value current thread saved */
    key_value_s_c = key_value_s_o;
    while (key_value_s_c != NULL) {
        if (key_value_s_c->key_value.thread == krhino_cur_task_get()) {
            RHINO_CRITICAL_EXIT();
            return (void*)key_value_s_c->key_value.value;
        }

        key_value_s_c = key_value_s_c->next;
    }

    /* if can not find the value current thread saved return NULL */
    RHINO_CRITICAL_EXIT();
    return NULL;
}

int pthread_key_delete(pthread_key_t key)
{
    pthread_key_list_t  *pthread_key_list_s_c = NULL;
    pthread_key_list_t  *pthread_key_list_s_l = NULL;
    pthread_key_value_t *key_value_s_o        = NULL;
    pthread_key_value_t *key_value_s_c        = NULL;
    pthread_key_value_t *key_value_s_n        = NULL;

    int list_flag = 0;

    CPSR_ALLOC();
    RHINO_CRITICAL_ENTER();

    /* if key saved in pthread_key_list_head */
    if (pthread_key_list_head.key_num == key) {
        key_value_s_c = pthread_key_list_head.head.next;

        /* free the value saved in the key */
        while (key_value_s_c != NULL) {
            key_value_s_n = key_value_s_c->next;
            krhino_mm_free(key_value_s_c);
            key_value_s_c = key_value_s_n;
        }

        /* set the list pointer to NULL */
        pthread_key_list_head.head.next = NULL;

        /* if no other key save in the list set key_num to 0 */
        if (pthread_key_list_head.next == NULL) {
            pthread_key_list_head.key_num = 0;
        } else { /* else copy the next key to the head */
            pthread_key_list_s_c = pthread_key_list_head.next;
            pthread_key_list_head.key_num = pthread_key_list_head.next->key_num;
            memcpy(&pthread_key_list_head.head, &pthread_key_list_head.next->head, sizeof(pthread_key_value_head_t));
            pthread_key_list_head.next = pthread_key_list_head.next->next;

            krhino_mm_free(pthread_key_list_s_c);
        }
    } else { /* if key is not saved in pthread_key_list_head, find the key in the list */
        /* find the key in the list */
        pthread_key_list_s_c = pthread_key_list_head.next;
        while (pthread_key_list_s_c != NULL) {
            if (pthread_key_list_s_c->key_num == key) {
                key_value_s_o = pthread_key_list_s_c->head.next;

                if (pthread_key_list_s_l == NULL) {
                    pthread_key_list_head.next = pthread_key_list_s_c->next;
                } else {
                    pthread_key_list_s_l->next = pthread_key_list_s_c->next;
                }

                krhino_mm_free(pthread_key_list_s_c);

                list_flag = 1;
                break;
            }

            pthread_key_list_s_l = pthread_key_list_s_c;
            pthread_key_list_s_c = pthread_key_list_s_c->next;
        }

        /* if can not find the key in list return error */
        if (list_flag == 0) {
            RHINO_CRITICAL_EXIT();
            return -1;
        }

        /* if no value saved in the key return */
        if (key_value_s_o == NULL) {
            RHINO_CRITICAL_EXIT();
            return 0;
        }

        key_value_s_c = key_value_s_o;
        while (key_value_s_c != NULL) {
            key_value_s_n = key_value_s_c->next;
            krhino_mm_free(key_value_s_c);
            key_value_s_c = key_value_s_n;
        }

        pthread_key_list_s_c->head.next = NULL;
    }

    RHINO_CRITICAL_EXIT();
    return 0;
}


void krhino_task_del_hook(ktask_t *task, res_free_t *arg)
{
    _pthread_tcb_t *ptcb;
    _pthread_cleanup_t *cleanup;
    pthread_key_list_t *pthread_key_list_s_c = NULL;

    g_sched_lock[cpu_cur_get()]++;

    ptcb = _pthread_get_tcb(task);

    if (ptcb == NULL) {
        g_sched_lock[cpu_cur_get()]--;
        return;
    }

    /* excute all cleanup function if existed */
    do {
        cleanup = ptcb->cleanup;
        if (cleanup != NULL ) {
            ptcb->cleanup = cleanup->prev;
            cleanup->cleanup_routine(cleanup->para);
            krhino_mm_free(cleanup);
        }
    } while(ptcb->cleanup != NULL);

    /* call the destructor function of TSD */
    pthread_key_list_s_c = &pthread_key_list_head;
    while (pthread_key_list_s_c != NULL) {
        if (pthread_key_list_s_c->head.fun != NULL) {
            pthread_key_list_s_c->head.fun(NULL);
        }

        pthread_key_list_s_c = pthread_key_list_s_c->next;
    }

    if (ptcb->attr.detachstate == PTHREAD_CREATE_JOINABLE) {
        /* give join sem if is joinable */
        if (ptcb->join_sem != NULL) {
            krhino_sem_give(ptcb->join_sem);
        }
    }

    if (arg == NULL) {
        g_sched_lock[cpu_cur_get()]--;
        return;
    }

    if (ptcb->attr.detachstate == PTHREAD_CREATE_DETACHED) {
        if (ptcb->join_sem != NULL) {
            krhino_sem_dyn_del(ptcb->join_sem);
        }

        klist_insert(&g_res_list, &arg->res_list);
        arg->res[0] = task->task_stack_base;
        arg->res[1] = task;
        arg->res[2] = ptcb;
        arg->cnt += 3;
        krhino_sem_give(&g_res_sem);
    }

    g_sched_lock[cpu_cur_get()]--;
}


#endif
