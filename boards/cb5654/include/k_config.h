/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */

#ifndef CONFIG_H
#define CONFIG_H


#if defined (__CSKY_DSP__) || defined(__CSKY_DSPV2__) || defined(__CSKY_VDSPV2__) || defined (__CSKY_HARD_FLOAT__) || defined(__C807__)
#define CSK_CPU_STACK_EXTRAL    68
#define RHINO_CONFIG_MM_MAXMSIZEBIT 27
#elif defined (__I805__)
#define CSK_CPU_STACK_EXTRAL    260
#else
#define CSK_CPU_STACK_EXTRAL    0
#endif

#define RHINO_CONFIG_STD_MALLOC              1
#define K_MM_STATISTIC                       1

/* chip level conf */
#define RHINO_CONFIG_LITTLE_ENDIAN           1
#define RHINO_CONFIG_CPU_STACK_DOWN          1

/* kernel feature conf */
#define RHINO_CONFIG_SEM                     1
#define RHINO_CONFIG_QUEUE                   1
#define RHINO_CONFIG_TASK_SEM                1
#define RHINO_CONFIG_EVENT_FLAG              1
#define RHINO_CONFIG_TIMER                   1
#define RHINO_CONFIG_BUF_QUEUE               1
#define RHINO_CONFIG_MM_BLK                  1
#define RHINO_CONFIG_MM_TLF_BLK_SIZE         4096

#ifdef CONFIG_DEBUG_MM
#define RHINO_CONFIG_MM_DEBUG                1
#define RHINO_CONFIG_GCC_RETADDR             1
#else
#define RHINO_CONFIG_MM_DEBUG                0
#define RHINO_CONFIG_GCC_RETADDR             0
#endif

#define RHINO_CONFIG_KOBJ_SET                1
#define RHINO_CONFIG_RINGBUF_VENDOR          0

/* kernel dynamic tick conf */
#ifdef CONFIG_KERNEL_PWR_MGMT
#define RHINO_CONFIG_CPU_PWR_MGMT            1
#else
#define RHINO_CONFIG_CPU_PWR_MGMT            0
#endif

/*
#define RHINO_CONFIG_TICKS_PER_SECOND        100
#define RHINO_CONFIG_TICK_HEAD_ARRAY         1
#define RHINO_CONFIG_SCHED_RR                0
#define RHINO_CONFIG_TIMER_RATE              1
#define RHINO_CONFIG_TIMER                   1
*/

/* kernel task conf */
#define RHINO_CONFIG_TASK_SUSPEND            1

#define RHINO_CONFIG_TASK_DEL                1
#define RHINO_CONFIG_TASK_WAIT_ABORT         0
#define RHINO_CONFIG_TASK_STACK_OVF_CHECK    1

#define RHINO_CONFIG_SCHED_RR                1

#define RHINO_CONFIG_TIME_SLICE_DEFAULT      50
#define RHINO_CONFIG_PRI_MAX                 62
#define RHINO_CONFIG_USER_PRI_MAX            (RHINO_CONFIG_PRI_MAX - 2)

#define RHINO_CONFIG_MM_REGION_MUTEX         0

/* kernel workqueue conf */
#ifdef CONFIG_KERNEL_WORKQUEUE
#define RHINO_CONFIG_WORKQUEUE               1
#else
#define RHINO_CONFIG_WORKQUEUE               0
#endif

/* kernel timer&tick conf */
#define RHINO_CONFIG_HW_COUNT                0

#ifdef CONFIG_KERNEL_SYS_STATS               
#define RHINO_CONFIG_SYS_STATS               CONFIG_KERNEL_SYS_STATS
#else
#define RHINO_CONFIG_SYS_STATS               0
#endif

#if RHINO_CONFIG_SYS_STATS
#undef RHINO_CONFIG_HW_COUNT
#define RHINO_CONFIG_HW_COUNT                1
#endif

#define RHINO_CONFIG_TICK_TASK               1
#if (RHINO_CONFIG_TICK_TASK > 0)
#define RHINO_CONFIG_TICK_TASK_STACK_SIZE    (50 + CSK_CPU_STACK_EXTRAL)
#define RHINO_CONFIG_TICK_TASK_PRI           1
#endif

#ifdef CONFIG_SYSTICK_HZ
#define RHINO_CONFIG_TICKS_PER_SECOND        CONFIG_SYSTICK_HZ
#else
#define RHINO_CONFIG_TICKS_PER_SECOND        100
#endif

/* must be 2^n size!, such as 1, 2, 4, 8, 16,32, etc....... */
#define RHINO_CONFIG_TICK_HEAD_ARRAY         1

#ifdef CONFIG_KERNEL_TIMER_STACK_SIZE
#define RHINO_CONFIG_TIMER_TASK_STACK_SIZE   (CONFIG_KERNEL_TIMER_STACK_SIZE/4)
#else
#define RHINO_CONFIG_TIMER_TASK_STACK_SIZE   (200 + CSK_CPU_STACK_EXTRAL)
#endif

#define RHINO_CONFIG_TIMER_RATE              1
#define RHINO_CONFIG_TIMER_TASK_PRI          5

#ifdef CONFIG_KERNEL_TIMER_MSG_NUM
#define RHINO_CONFIG_TIMER_MSG_NUM CONFIG_KERNEL_TIMER_MSG_NUM
#endif

/* kernel intrpt conf */
#define RHINO_CONFIG_INTRPT_STACK_REMAIN_GET 1
#define RHINO_CONFIG_INTRPT_STACK_OVF_CHECK  0
#define RHINO_CONFIG_INTRPT_MAX_NESTED_LEVEL 188u
#define RHINO_CONFIG_INTRPT_GUARD            0
#define RHINO_CONFIG_STACK_OVF_CHECK_HW      0

#define RHINO_CONFIG_KOBJ_DYN_ALLOC          1
#if (RHINO_CONFIG_KOBJ_DYN_ALLOC > 0)
#define RHINO_CONFIG_K_DYN_QUEUE_MSG         30
#define RHINO_CONFIG_K_DYN_TASK_STACK        (64 + CSK_CPU_STACK_EXTRAL)
#define RHINO_CONFIG_K_DYN_MEM_TASK_PRI      RHINO_CONFIG_USER_PRI_MAX
#endif

/* kernel idle conf */
#ifdef CONFIG_IDLE_TASK_STACK_SIZE
#define RHINO_CONFIG_IDLE_TASK_STACK_SIZE    CONFIG_IDLE_TASK_STACK_SIZE
#else
#define RHINO_CONFIG_IDLE_TASK_STACK_SIZE    (256 + CSK_CPU_STACK_EXTRAL)
#endif
/* kernel hook conf */
#define RHINO_CONFIG_USER_HOOK               1
#define RHINO_CONFIG_TASK_INFO               1
#define RHINO_CONFIG_TASK_INFO_NUM           3

/* kernel lwip hook conf for LWIP_NETCONN_SEM_PER_THREAD */
#ifdef CONFIG_KERNEL_LWIP_HOOK               
#define RHINO_CONFIG_USER_HOOK_FOR_LWIP      CONFIG_KERNEL_LWIP_HOOK
#else
#define RHINO_CONFIG_USER_HOOK_FOR_LWIP      0
#endif

#if RHINO_CONFIG_USER_HOOK_FOR_LWIP
#undef RHINO_CONFIG_TASK_INFO
#undef RHINO_CONFIG_TASK_INFO_NUM
#define RHINO_CONFIG_TASK_INFO               1
/* task tcb user_info num for user data store, No.3 for lwip sem */
#define RHINO_CONFIG_TASK_INFO_NUM           3
#endif

/* kernel stats conf */
#define RHINO_CONFIG_SYSTEM_STATS            1
#define RHINO_CONFIG_DISABLE_SCHED_STATS     0
#define RHINO_CONFIG_DISABLE_INTRPT_STATS    0
#define RHINO_CONFIG_CPU_USAGE_STATS         1
#define RHINO_CONFIG_CPU_USAGE_TASK_PRI      (RHINO_CONFIG_PRI_MAX - 2)
#define RHINO_CONFIG_CPU_USAGE_TASK_STACK    (50 + CSK_CPU_STACK_EXTRAL)

#endif /* CONFIG_H */

