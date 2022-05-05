/*
 * Copyright (C) 2015-2020 Alibaba Group Holding Limited
 */
#include <stdio.h>
#include <aos/kernel.h>
#include <aos/hal/touch.h>
#include <ulog/ulog.h>
#include "d1_port.h"
#include "app_main.h"
#include <aos/cli.h>

#define TAG "main"
extern void cxx_system_init(void);
extern void touchscreen_event(touch_message_t touch_msg);
extern int falcon_entry(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    cxx_system_init();
    board_yoc_init();
    hal_touch_init(touchscreen_event);
    printf("haasui entry here!\r\n");
    printf("haasui build time: %s, %s\r\n", __DATE__, __TIME__);

    falcon_entry(0, NULL);

    while (1) {
        aos_msleep(2000);
    };
}