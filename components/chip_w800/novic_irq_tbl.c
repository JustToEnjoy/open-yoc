/*
 * Copyright (C) 2016 YunOS Project. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     novic_irq_tbl.c
 * @brief    source file for the lib
 * @version  V1.0
 * @date     02. June 2017
 * @vendor   csky
 * @chip     pangu
 ******************************************************************************/
#include <csi_config.h>
#include <soc.h>

extern void Default_Handler(void);
extern void NOVIC_IRQ_Default_Handler(void);

void (*g_irqvector[32])(void);

void irq_vectors_init(void)
{
    int i;

    for (i = 0; i < (sizeof(g_irqvector) / sizeof(void *)); i++) {
        csi_vic_set_vector(i, (uint32_t)NOVIC_IRQ_Default_Handler);
        g_irqvector[i] = Default_Handler;
    }
}
