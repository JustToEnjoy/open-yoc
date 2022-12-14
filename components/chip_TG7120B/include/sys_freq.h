/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     sys_freq.h
 * @brief    header file for setting system frequency.
 * @version  V1.0
 * @date     18. July 2018
 ******************************************************************************/
#ifndef _SYS_FREQ_H_
#define _SYS_FREQ_H_

#include <stdint.h>
#include "soc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IHS_CLK       = 0,         /* internal high speed clock */
    EHS_CLK       = 1,         /* external high speed clock */
    ILS_CLK       = 2,         /* internal low speed clock */
    ELS_CLK       = 3          /* enternal low speed clock */
} clk_src_e;

typedef enum {
    OSR_8M_CLK_16M      = 0x80204,
    OSR_8M_CLK_24M      = 0x80206,
    OSR_8M_CLK_32M      = 0x80208,
    OSR_8M_CLK_40M      = 0x8020a,
    OSR_8M_CLK_48M      = 0x8020c
} clk_val_e;

/**
  \brief       set sys freq
  \param[in]   source \ref clk_src_e
  \param[in]   val    \ref clk_val_e
*/
void drv_set_sys_freq(clk_src_e source, clk_val_e val);

/**
  \brief       config rtc clock source
  \param[in]   source \ref clk_src_e, only support ILS_CLK and ELS_CLK
*/
void drv_rtcclk_config(clk_src_e source);

/**
  \brief       get eflash write/program latency
  \param[out]  prog   return write latency
  \param[out]  erase  return program latency
*/
void drv_get_eflash_params(clk_val_e val, uint16_t *prog, uint16_t *erase);

/**
  \brief       get sys freq
  return       frequency value
*/
int32_t drv_get_sys_freq(void);

/**
  \brief       get apb freq
  return       frequency value
*/
int32_t drv_get_apb_freq(int32_t idx);

/**
  \brief       get rtc freq
  return       frequency value
*/
int32_t drv_get_rtc_freq(void);

/**
  \brief       get timer freq
  return       frequency value
*/
int32_t drv_get_timer_freq(int32_t idx);

int32_t drv_get_cur_cpu_freq();

#ifdef __cplusplus
}
#endif

#endif /* _SYS_FREQ_H_ */

