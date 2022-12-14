/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     board_init.c
 * @brief    CSI Source File for board init
 * @version  V1.0
 * @date     31. June 2018
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>

#include <soc.h>
#include <csi_core.h>
#include <silan_syscfg.h>

#include <drv/usart.h>
#include <pin_name.h>
#include <pinmux.h>

#include "board.h"

extern void ioreuse_initial(void);
static void board_pinmux_config(void)
{
    //console
    drv_pinmux_config(PA4, PA4_UART2_TX);
    drv_pinmux_config(PA5, PA5_UART2_RX);

    // BT
    drv_pinmux_config(PB0, PB0_UART1_TX);
    drv_pinmux_config(PB1, PB1_UART1_RX);

    //WiFi
    drv_pinmux_config(PC2, PC2_SD_D0);
    drv_pinmux_config(PC1, PC1_SD_D1);
    drv_pinmux_config(PC6, PC6_SD_D2);
    drv_pinmux_config(PC5, PC5_SD_D3);
    drv_pinmux_config(PC3, PC3_SD_CLK);
    drv_pinmux_config(PC4, PC4_SD_CMD_CMD);
    drv_pinmux_config(PC0, PC0_SD_DET);

    //SD card
    drv_pinmux_config(PC9, PC9_SDIO_D0);
    drv_pinmux_config(PC8, PC8_SDIO_D1);
    drv_pinmux_config(PC13, PC13_SDIO_D2);
    drv_pinmux_config(PC12, PC12_SDIO_D3);
    drv_pinmux_config(PC10, PC10_SDIO_CLK);
    drv_pinmux_config(PC11, PC11_SDIO_CMD);
    drv_pinmux_config(PC7, PC7_SDIO_DET);
}

void board_init(void)
{
    ioreuse_initial();

    board_pinmux_config();

    /* close m0/dsp debug */
    silan_mcu_debug_close();
    silan_dsp_debug_close();
}
