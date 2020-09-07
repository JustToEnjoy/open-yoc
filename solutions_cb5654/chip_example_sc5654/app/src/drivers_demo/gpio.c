/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
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

#include <app_config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <pin_name.h>
#include <pinmux.h>
#include <drv/gpio.h>

#include <aos/cli.h>

volatile static bool int_flag = 1;

static void gpio_interrupt_handler(int32_t idx)
{
    int_flag = 0;
}

void gpio_output_test(int gpio_id, int val)
{
    gpio_pin_handle_t     handle;

    drv_pinmux_config(gpio_id, PIN_FUNC_GPIO);
    handle = csi_gpio_pin_initialize(gpio_id, NULL);
    csi_gpio_pin_config_mode(handle, GPIO_MODE_PUSH_PULL);
    csi_gpio_pin_config_direction(handle, GPIO_DIRECTION_OUTPUT);
    csi_gpio_pin_write(handle, val);
    csi_gpio_pin_uninitialize(handle);
}

void gpio_interrupt_test(int gpio_id)
{
    gpio_pin_handle_t     handle;

    drv_pinmux_config(gpio_id, PIN_FUNC_GPIO);
    handle = csi_gpio_pin_initialize(gpio_id, gpio_interrupt_handler);
    csi_gpio_pin_config_mode(handle, GPIO_MODE_PULLNONE);
    csi_gpio_pin_config_direction(handle, GPIO_DIRECTION_INPUT);
    csi_gpio_pin_set_irq(handle, GPIO_IRQ_MODE_RISING_EDGE, 1);

    while (int_flag);

    int_flag = 1;
    csi_gpio_pin_uninitialize(handle);
}

bool gpio_input_test(int gpio_id)
{
    gpio_pin_handle_t     handle;
    bool val = 0;

    drv_pinmux_config(gpio_id, PIN_FUNC_GPIO);
    handle = csi_gpio_pin_initialize(gpio_id, NULL);
    csi_gpio_pin_config_mode(handle, GPIO_MODE_PULLNONE);
    csi_gpio_pin_config_direction(handle, GPIO_DIRECTION_INPUT);
    csi_gpio_pin_read(handle, &val);
    csi_gpio_pin_uninitialize(handle);

    return val;
}

static void cmd_app_func(char *wbuf, int wbuf_len, int argc, char **argv)
{
    int cmd_err = 0;
    if (argc < 3) {
        cmd_err = 1;
    }

    if (cmd_err == 0) {
        if (strcmp(argv[1], "set") == 0) {
            if (argc == 4) {
                gpio_output_test(atoi(argv[2]), atoi(argv[3]));
                printf("set gpio %d val=%d\n", atoi(argv[2]), atoi(argv[3]));
            } else {
                cmd_err = 1;
            }
        } else if (strcmp(argv[1], "get") == 0) {
            bool val = gpio_input_test(atoi(argv[2]));
            printf("get gpio %d val=%d\n", atoi(argv[2]), val);
        } else if (strcmp(argv[1], "irq") == 0) {
            gpio_interrupt_test(atoi(argv[2]));
            printf("gpio %d irq\n", atoi(argv[2]));
        }
    }

    if (cmd_err) {
        printf("usage:\n");
        printf("\tgpio set pinid 0/1\n");
        printf("\tgpio get pinid\n");
        printf("\tgpio irq pinid\n");
    }
}

void test_gpio_cmd(void)
{
    static const struct cli_command cmd_info = {"gpio", "gpio output/input/irq", cmd_app_func};

    aos_cli_register_command(&cmd_info);
}
