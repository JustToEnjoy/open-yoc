#include <stdbool.h>
#include <aos/aos.h>
#include <aos/kv.h>
#include <yoc/yoc.h>
#include <yoc/partition.h>
#include <devices/devicelist.h>
#include <board_config.h>

#include "pin_name.h"
#include "app_init.h"

const char *TAG = "INIT";

#ifndef CONSOLE_IDX
#define CONSOLE_IDX 0
#endif

void board_yoc_init()
{
    int ret;

    board_init();

    console_init(CONSOLE_UART_IDX, 115200, 128);

    ulog_init();
    aos_set_log_level(AOS_LL_DEBUG);

    /* load partition */
    ret = partition_init();

    if (ret <= 0) {
        LOGE(TAG, "partition init failed");
    } else {
        LOGI(TAG, "find %d partitions", ret);
    }

    /* kvfs init */
    aos_kv_init("kv");

    LOGI(TAG, "Build:%s,%s", __DATE__, __TIME__);
}

