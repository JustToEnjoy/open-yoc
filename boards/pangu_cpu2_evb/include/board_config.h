/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     board_config.h
 * @brief    header File for pin definition
 * @version  V1.0
 * @date     02. June 2018
 ******************************************************************************/

#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <stdint.h>

#ifdef CONFIG_CSI_V2
#include "drv/pin.h"
#else
#include "pin_name.h"
#include "pinmux.h"
#endif

#include "soc.h"

#ifdef __cplusplus
extern "C" {
#endif

#if 1
#define CONSOLE_TXD         PA19
#define CONSOLE_RXD         PA20
#define CONSOLE_TXD_FUNC    PA19_UART1_TX
#define CONSOLE_RXD_FUNC    PA20_UART1_RX
#define CONSOLE_UART_IDX    1
#else
#define CONSOLE_TXD         PA2
#define CONSOLE_RXD         PA0
#define CONSOLE_TXD_FUNC    PA2_UART0_TX
#define CONSOLE_RXD_FUNC    PA0_UART0_RX
#define CONSOLE_UART_IDX    0
#endif

#define QSPIFLASH_IDX                   0
#define EXAMPLE_QSPI_IDX                0

#define USI0_SCLK       PA21
#define USI0_SD0        PA22
#define USI0_IDX  0

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_CONFIG_H_ */

