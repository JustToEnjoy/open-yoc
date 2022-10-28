/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     system.c
 * @brief    CSI Device System Source File
 * @version  V1.0
 * @date     7. April 2020
 ******************************************************************************/

#include <soc.h>
#include <csi_core.h>
#include <csi_config.h>
#include <drv/io.h>
#include <drv/irq.h>
#include <drv/dma.h>
#include <drv/tick.h>
#include <drv/etb.h>
#include <drv/spiflash.h>

extern uint32_t g_top_irqstack;
void section_data_copy(void);
void section_ram_code_copy(void);
void section_bss_clear(void);

#define CACHE_BASSE_ADDR 0 //cache base addr

void cache_init(void)
{
#ifdef CONFIG_XIP
    csi_cache_set_range(0U, CACHE_BASSE_ADDR, (uint32_t)CACHE_CRCR_8M, 1U);
    csi_icache_enable();
    csi_dcache_enable();
#else
    csi_cache_set_range(0U, CACHE_BASSE_ADDR, (uint32_t)CACHE_CRCR_128K, 1U);
    csi_icache_enable();
#endif
}

static void section_init(void)
{
#ifdef CONFIG_XIP
    section_data_copy();
    section_ram_code_copy();
#endif

    section_bss_clear();
}

static void vic_init(void)
{
    /* Clear active and pending IRQ, and disable IRQ */
    VIC->IABR[0] = 0x0U;
    VIC->ICPR[0] = 0xFFFFFFFFU;
    VIC->ICER[0] = 0xFFFFFFFFU;
    VIC->IABR[1] = 0x0U;
    VIC->ICPR[1] = 0xFFFFFFFFU;
    VIC->ICER[1] = 0xFFFFFFFFU;
}

static void interrupt_init(void)
{
    vic_init();

#ifdef CONFIG_KERNEL_NONE
    __enable_excp_irq();
#else
#if defined(CONFIG_SUPPORT_TSPEND)
    __set_Int_SP((uint32_t)&g_top_irqstack);
    __set_CHR(__get_CHR() | CHR_ISE_Msk);
    VIC->TSPR = 0xFF;
#endif
#endif
}

/**
  * @brief  initialize the system
  * @param  None
  * @return None
  */
__attribute__((weak)) void SystemInit(void)
{
    section_init();

    interrupt_init();

    soc_set_sys_freq(1000000000);

    /* cache_init(); */

    csi_tick_init();
}
