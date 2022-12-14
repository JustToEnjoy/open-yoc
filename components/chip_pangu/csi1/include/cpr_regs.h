/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     cpr_regs.h
 * @brief    header file for the cpr
 * @version  V1.0
 * @date     07. Aug 2019
 * @vendor   csky
 * @chip     pangu
 ******************************************************************************/

#ifndef _CPR_REGS_H_
#define _CPR_REGS_H_

#include <csi_core.h>

/* CPR0_CPU0CLK_SEL, Offset: 0x0 */
#define CPR0_CPU0CLK_SEL_CPU0CLK_SEL_Pos        0U
#define CPR0_CPU0CLK_SEL_CPU0CLK_SEL_Msk        (0x1U << CPR0_CPU0CLK_SEL_CPU0CLK_SEL_Pos)
#define CPR0_CPU0CLK_SEL_CPU0CLK_SEL_EHS        CPR0_CPU0CLK_SEL_CPU0CLK_SEL_Msk

/* CPR0_SYSCLK0_DIV_CTL, Offset: 0x4 */
#define CPR0_SYSCLK0_DIV_CTL_APB0_SUB1_DIV_Pos  0U
#define CPR0_SYSCLK0_DIV_CTL_APB0_SUB1_DIV_Msk  (0x3U << CPR0_SYSCLK0_DIV_CTL_APB0_SUB1_DIV_Pos)
#define CPR0_SYSCLK0_DIV_CTL_APB0_SUB1_DIV(n)   (((n) >> 1) << CPR0_SYSCLK0_DIV_CTL_APB0_SUB1_DIV_Pos)
#define CPR0_SYSCLK0_DIV_CTL_APB0_SUB2_DIV_Pos  4U
#define CPR0_SYSCLK0_DIV_CTL_APB0_SUB2_DIV_Msk  (0x3U << CPR0_SYSCLK0_DIV_CTL_APB0_SUB2_DIV_Pos)
#define CPR0_SYSCLK0_DIV_CTL_APB0_SUB2_DIV(n)   (((n) >> 1) << CPR0_SYSCLK0_DIV_CTL_APB0_SUB2_DIV_Pos)
#define CPR0_SYSCLK0_DIV_CTL_APB0_DIV_Pos       8U
#define CPR0_SYSCLK0_DIV_CTL_APB0_DIV_Msk       (0xFU << CPR0_SYSCLK0_DIV_CTL_APB0_DIV_Pos)
#define CPR0_SYSCLK0_DIV_CTL_APB0_DIV(n)        (((n) - 1) << CPR0_SYSCLK0_DIV_CTL_APB0_DIV_Pos)

/* CPR0_FFT_CTL, Offset: 0xC */
#define CPR0_FFT_CTL_FFT_CG_EN_Pos              0U
#define CPR0_FFT_CTL_FFT_CG_EN_Msk              (0x1U << CPR0_FFT_CTL_FFT_CG_EN_Pos)
#define CPR0_FFT_CTL_FFT_CG_EN                  CPR0_FFT_CTL_FFT_CG_EN_Msk
#define CPR0_FFT_CTL_FFT_BUSY_Pos               4U
#define CPR0_FFT_CTL_FFT_BUSY_Msk               (0x1U << CPR0_FFT_CTL_FFT_BUSY_Pos)
#define CPR0_FFT_CTL_FFT_BUSY                   CPR0_FFT_CTL_FFT_BUSY_Msk

/* CPR0_TIM1_CTRL, Offset: 0x10 */
#define CPR0_FFT_CTL_TIM1_CG_EN_Pos             0U
#define CPR0_FFT_CTL_TIM1_CG_EN_Msk             (0x1U << CPR0_FFT_CTL_TIM1_CG_EN_Pos)
#define CPR0_FFT_CTL_TIM1_CG_EN                 CPR0_FFT_CTL_TIM1_CG_EN_Msk
#define CPR0_FFT_CTL_TIM1_SERIE_SEL_Pos         4U
#define CPR0_FFT_CTL_TIM1_SERIE_SEL_Msk         (0x1U << CPR0_FFT_CTL_TIM1_SERIE_SEL_Pos)
#define CPR0_FFT_CTL_TIM1_SERIE_SEL             CPR0_FFT_CTL_TIM1_SERIE_SEL_Msk
#define CPR0_FFT_CTL_TIM1_BUSY_Pos              8U
#define CPR0_FFT_CTL_TIM1_BUSY_Msk              (0x1U << CPR0_FFT_CTL_TIM1_BUSY_Pos)

/* CPR0_FMC_AHB0_CTL, Offset: 0x14 */
//TODO

/* CPR0_SDIO_CLK_CTL, Offset: 0x44 */
#define CPR0_SDIO_CLK_CTL_SDIO_CG_EN_Pos        0U
#define CPR0_SDIO_CLK_CTL_SDIO_CG_EN_Msk        (0x1U << CPR0_SDIO_CLK_CTL_SDIO_CG_EN_Pos)
#define CPR0_SDIO_CLK_CTL_SDIO_CG_EN            CPR0_SDIO_CLK_CTL_SDIO_CG_EN_Msk
#define CPR0_SDIO_CLK_CTL_SDIO_SEL_Pos          4U
#define CPR0_SDIO_CLK_CTL_SDIO_SEL_Msk          (0x1U << CPR0_SDIO_CLK_CTL_SDIO_SEL_Pos)
#define CPR0_SDIO_CLK_CTL_SDIO_SEL_EHS          CPR0_SDIO_CLK_CTL_SDIO_SEL_Msk
#define CPR0_SDIO_CLK_CTL_SDIO_DIV_Pos          8U
#define CPR0_SDIO_CLK_CTL_SDIO_DIV_Msk          (0x7U << CPR0_SDIO_CLK_CTL_SDIO_DIV_Pos)
#define CPR0_SDIO_CLK_CTL_SDIO_DIV(n)           ((n - 1) << CPR0_SDIO_CLK_CTL_SDIO_DIV_Pos)
#define CPR0_SDIO_CLK_CTL_SDIO_BUSY_Pos         12U
#define CPR0_SDIO_CLK_CTL_SDIO_BUSY_Msk         (0x1U << CPR0_SDIO_CLK_CTL_SDIO_BUSY_Pos)
#define CPR0_SDIO_CLK_CTL_SDIO_BUSY             CPR0_SDIO_CLK_CTL_SDIO_BUSY_Msk

/* CPR0_MS0SWRST, Offset: 0x68 */
#define CPR0_MS0SWRST_ROM_RST_EN_Pos            0U
#define CPR0_MS0SWRST_ROM_RST_EN_Msk            (0x1U << CPR0_MS0SWRST_ROM_RST_EN_Pos)
#define CPR0_MS0SWRST_ROM_RST_EN                CPR0_MS0SWRST_ROM_RST_EN_Msk
#define CPR0_MS0SWRST_FMC0_RST_EN_Pos           1U
#define CPR0_MS0SWRST_FMC0_RST_EN_Msk           (0x1U << CPR0_MS0SWRST_FMC0_RST_EN_Pos)
#define CPR0_MS0SWRST_FMC0_RST_EN               CPR0_MS0SWRST_FMC0_RST_EN_Msk
#define CPR0_MS0SWRST_QSPI_RST_EN_Pos           2U
#define CPR0_MS0SWRST_QSPI_RST_EN_Msk           (0x1U << CPR0_MS0SWRST_QSPI_RST_EN_Pos)
#define CPR0_MS0SWRST_QSPI_RST_EN               CPR0_MS0SWRST_QSPI_RST_EN_Msk
#define CPR0_MS0SWRST_DMA0_RST_EN_Pos           3U
#define CPR0_MS0SWRST_DMA0_RST_EN_Msk           (0x1U << CPR0_MS0SWRST_DMA0_RST_EN_Pos)
#define CPR0_MS0SWRST_DMA0_RST_EN               CPR0_MS0SWRST_DMA0_RST_EN_Msk

/* CPR0_AHB0SUB0SWRST, Offset: 0x6C */
#define CPR0_AHB0SUB0SWRST_SASC_RST_EN_Pos      0U
#define CPR0_AHB0SUB0SWRST_SASC_RST_EN_Msk      (0x1U << CPR0_AHB0SUB0SWRST_SASC_RST_EN_Pos)
#define CPR0_AHB0SUB0SWRST_SASC_RST_EN          CPR0_AHB0SUB0SWRST_SASC_RST_EN_Msk
#define CPR0_AHB0SUB0SWRST_SDMMC_RST_EN_Pos     1U
#define CPR0_AHB0SUB0SWRST_SDMMC_RST_EN_Msk     (0x1U << CPR0_AHB0SUB0SWRST_SDMMC_RST_EN_Pos)
#define CPR0_AHB0SUB0SWRST_SDMMC_RST_EN         CPR0_AHB0SUB0SWRST_SDMMC_RST_EN_Msk
#define CPR0_AHB0SUB0SWRST_SDIO_RST_EN_Pos      2U
#define CPR0_AHB0SUB0SWRST_SDIO_RST_EN_Msk      (0x1U << CPR0_AHB0SUB0SWRST_SDIO_RST_EN_Pos)
#define CPR0_AHB0SUB0SWRST_SDIO_RST_EN          CPR0_AHB0SUB0SWRST_SDIO_RST_EN_Msk

/* CPR0_AHB0SUB1SWRST, Offset: 0x70 */
#define CPR0_AHB0SUB1SWRST_FFT_M_RST_EN_Pos     0U
#define CPR0_AHB0SUB1SWRST_FFT_M_RST_EN_Msk     (0x1U << CPR0_AHB0SUB1SWRST_FFT_M_RST_EN_Pos)
#define CPR0_AHB0SUB1SWRST_FFT_M_RST_EN         CPR0_AHB0SUB1SWRST_FFT_M_RST_EN_Msk
#define CPR0_AHB0SUB1SWRST_FFT_S_RST_EN_Pos     1U
#define CPR0_AHB0SUB1SWRST_FFT_S_RST_EN_Msk     (0x1U << CPR0_AHB0SUB1SWRST_FFT_S_RST_EN_Pos)
#define CPR0_AHB0SUB1SWRST_FFT_S_RST_EN         CPR0_AHB0SUB1SWRST_FFT_S_RST_EN_Msk
#define CPR0_AHB0SUB1SWRST_SHA_RST_EN_Pos       2U
#define CPR0_AHB0SUB1SWRST_SHA_RST_EN_Msk       (0x1U << CPR0_AHB0SUB1SWRST_SHA_RST_EN_Pos)
#define CPR0_AHB0SUB1SWRST_SHA_RST_EN           CPR0_AHB0SUB1SWRST_SHA_RST_EN_Msk
#define CPR0_AHB0SUB1SWRST_AES_RST_EN_Pos       3U
#define CPR0_AHB0SUB1SWRST_AES_RST_EN_Msk       (0x1U << CPR0_AHB0SUB1SWRST_AES_RST_EN_Pos)
#define CPR0_AHB0SUB1SWRST_AES_RST_EN           CPR0_AHB0SUB1SWRST_AES_RST_EN_Msk

/* CPR0_AHB0SUB2SWRST, Offset: 0x74 */
#define CPR0_AHB0SUB2SWRST_MCC0_RST_EN_Pos      0U
#define CPR0_AHB0SUB2SWRST_MCC0_RST_EN_Msk      (0x1U << CPR0_AHB0SUB2SWRST_MCC0_RST_EN_Pos)
#define CPR0_AHB0SUB2SWRST_MCC0_RST_EN          CPR0_AHB0SUB2SWRST_MCC0_RST_EN_Msk
#define CPR0_AHB0SUB2SWRST_RSA_RST_EN_Pos       1U
#define CPR0_AHB0SUB2SWRST_RSA_RST_EN_Msk       (0x1U << CPR0_AHB0SUB2SWRST_RSA_RST_EN_Pos)
#define CPR0_AHB0SUB2SWRST_RSA_RST_EN           CPR0_AHB0SUB2SWRST_RSA_RST_EN_Msk
#define CPR0_AHB0SUB2SWRST_TRNG_RST_EN_Pos      2U
#define CPR0_AHB0SUB2SWRST_TRNG_RST_EN_Msk      (0x1U << CPR0_AHB0SUB2SWRST_TRNG_RST_EN_Pos)
#define CPR0_AHB0SUB2SWRST_TRNG_RST_EN          CPR0_AHB0SUB2SWRST_TRNG_RST_EN_Msk

/* CPR0_APB0SWRST, Offset: 0x78 */
#define CPR0_APB0SWRST_QSPIC_RST_EN_Pos         0U
#define CPR0_APB0SWRST_QSPIC_RST_EN_Msk         (0x1U << CPR0_APB0SWRST_QSPIC_RST_EN_Pos)
#define CPR0_APB0SWRST_QSPIC_RST_EN             CPR0_APB0SWRST_QSPIC_RST_EN_Msk
#define CPR0_APB0SWRST_WDT_IN_RST_EN_Pos        1U
#define CPR0_APB0SWRST_WDT_IN_RST_EN_Msk        (0x1U << CPR0_APB0SWRST_WDT_IN_RST_EN_Pos)
#define CPR0_APB0SWRST_WDT_IN_RST_EN            CPR0_APB0SWRST_WDT_IN_RST_EN_Msk
#define CPR0_APB0SWRST_TIM1_RST_EN_Pos          2U
#define CPR0_APB0SWRST_TIM1_RST_EN_Msk          (0x1U << CPR0_APB0SWRST_TIM1_RST_EN_Pos)
#define CPR0_APB0SWRST_TIM1_RST_EN              CPR0_APB0SWRST_TIM1_RST_EN_Msk
#define CPR0_APB0SWRST_TIPC_RST_EN_Pos          3U
#define CPR0_APB0SWRST_TIPC_RST_EN_Msk          (0x1U << CPR0_APB0SWRST_TIPC_RST_EN_Pos)
#define CPR0_APB0SWRST_TIPC_RST_EN              CPR0_APB0SWRST_TIPC_RST_EN_Msk
#define CPR0_APB0SWRST_PWM0_RST_EN_Pos          4U
#define CPR0_APB0SWRST_PWM0_RST_EN_Msk          (0x1U << CPR0_APB0SWRST_PWM0_RST_EN_Pos)
#define CPR0_APB0SWRST_PWM0_RST_EN              CPR0_APB0SWRST_PWM0_RST_EN_Msk
#define CPR0_APB0SWRST_I2S0_RST_EN_Pos          5U
#define CPR0_APB0SWRST_I2S0_RST_EN_Msk          (0x1U << CPR0_APB0SWRST_I2S0_RST_EN_Pos)
#define CPR0_APB0SWRST_I2S0_RST_EN              CPR0_APB0SWRST_I2S0_RST_EN_Msk
#define CPR0_APB0SWRST_I2S1_RST_EN_Pos          6U
#define CPR0_APB0SWRST_I2S1_RST_EN_Msk          (0x1U << CPR0_APB0SWRST_I2S1_RST_EN_Pos)
#define CPR0_APB0SWRST_I2S1_RST_EN              CPR0_APB0SWRST_I2S1_RST_EN_Msk
#define CPR0_APB0SWRST_USI0_RST_EN_Pos          7U
#define CPR0_APB0SWRST_USI0_RST_EN_Msk          (0x1U << CPR0_APB0SWRST_USI0_RST_EN_Pos)
#define CPR0_APB0SWRST_USI0_RST_EN              CPR0_APB0SWRST_USI0_RST_EN_Msk
#define CPR0_APB0SWRST_USI1_RST_EN_Pos          8U
#define CPR0_APB0SWRST_USI1_RST_EN_Msk          (0x1U << CPR0_APB0SWRST_USI1_RST_EN_Pos)
#define CPR0_APB0SWRST_USI1_RST_EN              CPR0_APB0SWRST_USI1_RST_EN_Msk
#define CPR0_APB0SWRST_UART0_RST_EN_Pos         9U
#define CPR0_APB0SWRST_UART0_RST_EN_Msk         (0x1U << CPR0_APB0SWRST_UART0_RST_EN_Pos)
#define CPR0_APB0SWRST_UART0_RST_EN             CPR0_APB0SWRST_UART0_RST_EN_Msk
#define CPR0_APB0SWRST_UART1_RST_EN_Pos         10U
#define CPR0_APB0SWRST_UART1_RST_EN_Msk         (0x1U << CPR0_APB0SWRST_UART1_RST_EN_Pos)
#define CPR0_APB0SWRST_UART1_RST_EN             CPR0_APB0SWRST_UART1_RST_EN_Msk

typedef struct {
    __IOM uint32_t CPR0_CPU0CLK_SEL;
    __IOM uint32_t CPR0_SYSCLK0_DIV_CTL;
    __IOM uint32_t CPR0_RESERVED0;
    __IOM uint32_t CPR0_FFT_CTL;
    __IOM uint32_t CPR0_TIM1_CTL;
    __IOM uint32_t CPR0_FMC_AHB0_CTL;
    __IOM uint32_t CPR0_ROM_CTL;
    __IOM uint32_t CPR0_GPIO0_CTL;
    __IOM uint32_t CPR0_QSPI_CTL;
    __IOM uint32_t CPR0_USI0_CTL;
    __IOM uint32_t CPR0_USI1_CTL;
    __IOM uint32_t CPR0_UART0_CTL;
    __IOM uint32_t CPR0_I2S0_CLK_CTL;
    __IOM uint32_t CPR0_I2S1_CLK_CTL;
    __IOM uint32_t CPR0_EFUSE_CTL;
    __IOM uint32_t CPR0_SASC_CTL;
    __IOM uint32_t CPR0_TIPC_CTL;
    __IOM uint32_t CPR0_SDIO_CLK_CTL;
    __IOM uint32_t CPR0_SDMMC_CLK_CTL;
    __IOM uint32_t CPR0_AES_CLK_CTL;
    __IOM uint32_t CPR0_RSA_CLK_CTL;
    __IOM uint32_t CPR0_SHA_CLK_CTL;
    __IOM uint32_t CPR0_TRNG_CLK_CTL;
    __IOM uint32_t CPR0_DMA0_CLK_CTL;
    __IOM uint32_t CPR0_WDT_CLK_CTL;
    __IOM uint32_t CPR0_PWM0_CLK_CTL;
    __IOM uint32_t CPR0_MS0SWRST;
    __IOM uint32_t CPR0_AHB0SUB0SWRST;
    __IOM uint32_t CPR0_AHB0SUB1SWRST;
    __IOM uint32_t CPR0_AHB0SUB2SWRST;
    __IOM uint32_t CPR0_APB0SWRST;
    __IOM uint32_t CPR0_UART1_CTL;
    __IOM uint32_t CPR0_CK804_0_DAHBL_BASE;
    __IOM uint32_t CPR0_CK804_0_DAHBL_MASK;
    __IOM uint32_t CPR0_CK804_0_IAHBL_BASE;
    __IOM uint32_t CPR0_CK804_0_IAHBL_MASK;
    __IOM uint32_t CPR0_CK804_1_DAHBL_BASE;
    __IOM uint32_t CPR0_CK804_1_DAHBL_MASK;
    __IOM uint32_t CPR0_CK804_1_IAHBL_BASE;
    __IOM uint32_t CPR0_CK804_1_IAHBL_MASK;
    __IOM uint32_t CPR0_H2H0_REG0;
    __IOM uint32_t CPR0_H2H0_REG1;
    __IOM uint32_t CPR0_H2H0_REG2;
    __IOM uint32_t CPR0_H2H0_REG3;
} cpr0_reg_t;


/* CPR1_CPU2CLK_SEL, Offset: 0x0 */
#define CPR1_CPU2CLK_SEL_CPU2CLK_SEL_Pos     0U
#define CPR1_CPU2CLK_SEL_CPU2CLK_SEL_Msk     (0x1U << CPR1_CPU2CLK_SEL_CPU2CLK_SEL_Pos)
#define CPR1_CPU2CLK_SEL_CPU2CLK_SEL_EHS     CPR1_CPU2CLK_SEL_CPU2CLK_SEL_Msk

/* CPR1_SYSCLK1_DIV_CTL, Offset: 0x4 */
#define CPR1_SYSCLK1_DIV_CTL_APB1_DIV_Pos    0U
#define CPR1_SYSCLK1_DIV_CTL_APB1_DIV_Msk    (0xFU << CPR1_CPU2CLK_SEL_CPU2CLK_SEL_Pos)
#define CPR1_SYSCLK1_DIV_CTL_APB1_DIV(n)     (((n) - 1) << CPR1_SYSCLK1_DIV_CTL_APB1_DIV_Pos)

/* CPR1_MS1SWRST, Offset: 0x64 */
#define CPR1_MS1SWRST_FMC1_RST_EN_Pos        0U
#define CPR1_MS1SWRST_FMC1_RST_EN_Msk        (0x1U << CPR1_MS1SWRST_FMC1_RST_EN_Pos)
#define CPR1_MS1SWRST_FMC1_RST_EN            CPR1_MS1SWRST_FMC1_RST_EN_Msk
#define CPR1_MS1SWRST_FMC_SUB_RST_EN_Pos     1U
#define CPR1_MS1SWRST_FMC_SUB_RST_EN_Msk     (0x1U << CPR1_MS1SWRST_FMC_SUB_RST_EN_Pos)
#define CPR1_MS1SWRST_FMC_SUB_RST_EN         CPR1_MS1SWRST_FMC_SUB_RST_EN_Msk
#define CPR1_MS1SWRST_MCC1_RST_EN_Pos        2U
#define CPR1_MS1SWRST_MCC1_RST_EN_Msk        (0x1U << CPR1_MS1SWRST_MCC1_RST_EN_Pos)
#define CPR1_MS1SWRST_MCC1_RST_EN            CPR1_MS1SWRST_MCC1_RST_EN_Msk
#define CPR1_MS1SWRST_DMA1_RST_EN_Pos        3U
#define CPR1_MS1SWRST_DMA1_RST_EN_Msk        (0x1U << CPR1_MS1SWRST_DMA1_RST_EN_Pos)
#define CPR1_MS1SWRST_DMA1_RST_EN            CPR1_MS1SWRST_DMA1_RST_EN_Msk

/* CPR1_APB1SWRST, Offset: 0x68 */
#define CPR1_APB1SWRST_CODEC_RST_EN_Pos      0U
#define CPR1_APB1SWRST_CODEC_RST_EN_Msk      (0x1U << CPR1_APB1SWRST_CODEC_RST_EN_Pos)
#define CPR1_APB1SWRST_CODEC_RST_EN          CPR1_APB1SWRST_CODEC_RST_EN_Msk
#define CPR1_APB1SWRST_SPDIF_RST_EN_Pos      1U
#define CPR1_APB1SWRST_SPDIF_RST_EN_Msk      (0x1U << CPR1_APB1SWRST_SPDIF_RST_EN_Pos)
#define CPR1_APB1SWRST_SPDIF_RST_EN          CPR1_APB1SWRST_SPDIF_RST_EN_Msk
#define CPR1_APB1SWRST_I2S2_RST_EN_Pos       2U
#define CPR1_APB1SWRST_I2S2_RST_EN_Msk       (0x1U << CPR1_APB1SWRST_I2S2_RST_EN_Pos)
#define CPR1_APB1SWRST_I2S2_RST_EN           CPR1_APB1SWRST_I2S2_RST_EN_Msk
#define CPR1_APB1SWRST_I2S3_RST_EN_Pos       3U
#define CPR1_APB1SWRST_I2S3_RST_EN_Msk       (0x1U << CPR1_APB1SWRST_I2S3_RST_EN_Pos)
#define CPR1_APB1SWRST_I2S3_RST_EN           CPR1_APB1SWRST_I2S3_RST_EN_Msk
#define CPR1_APB1SWRST_TDM_RST_EN_Pos        4U
#define CPR1_APB1SWRST_TDM_RST_EN_Msk        (0x1U << CPR1_APB1SWRST_TDM_RST_EN_Pos)
#define CPR1_APB1SWRST_TDM_RST_EN            CPR1_APB1SWRST_TDM_RST_EN_Msk
#define CPR1_APB1SWRST_PDM_RST_EN_Pos        5U
#define CPR1_APB1SWRST_PDM_RST_EN_Msk        (0x1U << CPR1_APB1SWRST_PDM_RST_EN_Pos)
#define CPR1_APB1SWRST_PDM_RST_EN            CPR1_APB1SWRST_PDM_RST_EN_Msk
#define CPR1_APB1SWRST_USI2_RST_EN_Pos       6U
#define CPR1_APB1SWRST_USI2_RST_EN_Msk       (0x1U << CPR1_APB1SWRST_USI2_RST_EN_Pos)
#define CPR1_APB1SWRST_USI2_RST_EN           CPR1_APB1SWRST_USI2_RST_EN_Msk
#define CPR1_APB1SWRST_USI3_RST_EN_Pos       7U
#define CPR1_APB1SWRST_USI3_RST_EN_Msk       (0x1U << CPR1_APB1SWRST_USI3_RST_EN_Pos)
#define CPR1_APB1SWRST_USI3_RST_EN           CPR1_APB1SWRST_USI3_RST_EN_Msk
#define CPR1_APB1SWRST_GPIO1_RST_EN_Pos      8U
#define CPR1_APB1SWRST_GPIO1_RST_EN_Msk      (0x1U << CPR1_APB1SWRST_GPIO1_RST_EN_Pos)
#define CPR1_APB1SWRST_GPIO1_RST_EN          CPR1_APB1SWRST_GPIO1_RST_EN_Msk
#define CPR1_APB1SWRST_PWM1_RST_EN_Pos       9U
#define CPR1_APB1SWRST_PWM1_RST_EN_Msk       (0x1U << CPR1_APB1SWRST_PWM1_RST_EN_Pos)
#define CPR1_APB1SWRST_PWM1_RST_EN           CPR1_APB1SWRST_PWM1_RST_EN_Msk
#define CPR1_APB1SWRST_TIM2_RST_EN_Pos       10U
#define CPR1_APB1SWRST_TIM2_RST_EN_Msk       (0x1U << CPR1_APB1SWRST_TIM2_RST_EN_Pos)
#define CPR1_APB1SWRST_TIM2_RST_EN           CPR1_APB1SWRST_TIM2_RST_EN_Msk
#define CPR1_APB1SWRST_TIM3_RST_EN_Pos       11U
#define CPR1_APB1SWRST_TIM3_RST_EN_Msk       (0x1U << CPR1_APB1SWRST_TIM3_RST_EN_Pos)
#define CPR1_APB1SWRST_TIM3_RST_EN           CPR1_APB1SWRST_TIM3_RST_EN_Msk
#define CPR1_APB1SWRST_UART2_RST_EN_Pos      12U
#define CPR1_APB1SWRST_UART2_RST_EN_Msk      (0x1U << CPR1_APB1SWRST_UART2_RST_EN_Pos)
#define CPR1_APB1SWRST_UART2_RST_EN          CPR1_APB1SWRST_UART2_RST_EN_Msk
#define CPR1_APB1SWRST_UART3_RST_EN_Pos      13U
#define CPR1_APB1SWRST_UART3_RST_EN_Msk      (0x1U << CPR1_APB1SWRST_UART3_RST_EN_Pos)
#define CPR1_APB1SWRST_UART3_RST_EN          CPR1_APB1SWRST_UART3_RST_EN_Msk

typedef struct {
    __IOM uint32_t CPR1_CPU2CLK_SEL;
    __IOM uint32_t CPR1_SYSCLK1_DIV_CTL;
    __IOM uint32_t CPR1_TIM2_CTL;
    __IOM uint32_t CPR1_TIM3_CTL;
    __IOM uint32_t CPR1_GPIO1_CTL;
    __IOM uint32_t CPR1_USI2_CTL;
    __IOM uint32_t CPR1_USI3_CTL;
    __IOM uint32_t CPR1_UART2_CLK_CTL;
    __IOM uint32_t CPR1_UART3_CLK_CTL_REG ;
    __IOM uint32_t CPR1_I2S2_CTL;
    __IOM uint32_t CPR1_I2S3_CTL;
    __IOM uint32_t CPR1_SPDIF_CTL;
    __IOM uint32_t CPR1_FMC_AHB1_CTL;
    __IOM uint32_t CPR1_TDM_CTL;
    __IOM uint32_t CPR1_PDM_CTL;
    __IOM uint32_t CPR1_DMA1_CLK_CTL;
    __IOM uint32_t CPR1_PWM1_CLK_CTL;
    __IOM uint32_t CPR1_DMA_CH8_SEL;
    __IOM uint32_t CPR1_DMA_CH9_SEL;
    __IOM uint32_t CPR1_DMA_CH10_SEL;
    __IOM uint32_t CPR1_DMA_CH11_SEL;
    __IOM uint32_t CPR1_DMA_CH12_SEL;
    __IOM uint32_t CPR1_DMA_CH13_SEL;
    __IOM uint32_t CPR1_DMA_CH14_SEL;
    __IOM uint32_t CPR1_DMA_CH15_SEL;
    __IOM uint32_t CPR1_MS1SWRST;
    __IOM uint32_t CPR1_APB1SWRST;
    __IOM uint32_t CPR1_CODEC_CLK_CTL;
    __IOM uint32_t CPR1_H2H0_REG0;
    __IOM uint32_t CPR1_H2H0_REG1;
    __IOM uint32_t CPR1_H2H0_REG2;
    __IOM uint32_t CPR1_H2H0_REG3;
} cpr1_reg_t;

#endif
