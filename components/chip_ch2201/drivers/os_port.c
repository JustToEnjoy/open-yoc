/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */


#ifdef CONFIG_KERNEL_RHINO
extern void non_tspend_cpu_task_switch(void);
extern void non_tspend_cpu_intrpt_switch(void);
extern void non_tspend_cpu_first_task_start(void);

#ifndef CONFIG_SUPPORT_TSPEND
void cpu_task_switch(void)
{
    non_tspend_cpu_task_switch();
}

void cpu_intrpt_switch(void)
{
    non_tspend_cpu_intrpt_switch();
}

void cpu_first_task_start(void)
{
    non_tspend_cpu_first_task_start();
}
#endif /* CONFIG_SUPPORT_TSPEND */

#endif /* CONFIG_KERNEL_RHINO */
