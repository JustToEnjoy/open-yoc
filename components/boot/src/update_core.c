/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */
#if (CONFIG_NO_OTA_UPGRADE == 0)
#include "update.h"
#include <stdlib.h>
#include <string.h>
#include <yoc/partition.h>
#include "update_misc_man.h"
#include "update_log.h"
#include "update_mtb.h"
#include "update_diff.h"
#include "boot_flash_porting.h"
#include "boot_wrapper.h"

/****************************************************************************
* Definitions
****************************************************************************/
/****************************************************************************
 * Functions
 ****************************************************************************/

static int update_process(void)
{
    unsigned long fd, fd2;
    int ret = 0 ;
    uint32_t status = MISC_STATUS_IMG_SET;
    img_info_t img_f;
    unsigned long mantb = INVALID_ADDR;

#if (CONFIG_MANTB_VERSION < 4)
    mantb = mtb_get_addr(0); //primary mtb maybe modifying
#endif

    ret = misc_get_update_fd(&fd, &status);
    if (ret < 0) {
        UPD_LOGE("e continue fd:0x%lx", fd);
        return ret;
    }
    fd2 = fd;
    UPD_LOGD("continue fd:0x%lx,status:%d", fd, status);

    // check first
    UPD_LOGD("check the needed upgrade partition first.");
    while (1) {
        ret = misc_get_imager_info(fd, &img_f);
        if (ret < 0) {
            UPD_LOGE("e image");
            return UPDATE_CHECK_FAIL;
        }
        if (!boot_is_no_needed_ota((const char *)img_f.img_name)) {
            partition_t partition;
            partition_info_t *part_info;
            partition = partition_open((const char *)img_f.img_name);
            part_info = partition_info_get(partition);
            partition_close(partition);
            if (part_info->length % part_info->sector_size) {
                UPD_LOGE("[%s] partition length:0x%x is not align with sector:0x%x", (const char *)img_f.img_name, part_info->length, part_info->sector_size);
                return UPDATE_CHECK_FAIL;
            }
            if (img_f.img_size > part_info->length) {
                UPD_LOGE("[%s] image size too large:0x%x, 0x%x", (const char *)img_f.img_name, img_f.img_size, part_info->length);
                return UPDATE_CHECK_FAIL;
            }
        } else {
            UPD_LOGI("ota, skip %s.", (const char *)img_f.img_name);
        }
        fd = misc_next_imager(fd);
        if (!fd) {
            break;
        }
    }
    fd = fd2;
    UPD_LOGD("check the needed upgrade partition ok.");

    while (1) {
        ret = misc_get_imager_info(fd, &img_f);
        if (ret < 0) {
            UPD_LOGE("e image");
            return UPDATE_CHECK_FAIL;
        }

        if (!boot_is_no_needed_ota((const char *)img_f.img_name)) {
            ret = misc_update_path(mantb, &img_f , status, fd);
            if (ret < 0) {
                UPD_LOGE("e update");
                if (ret == UPDATE_NOT_SUPPORT) {
                    return UPDATE_NOT_SUPPORT;
                }
                return UPDATE_PATCH_FAIL;
            }
        } else {
            UPD_LOGI("ota, skip %s.", (const char *)img_f.img_name);
        }

        fd = misc_next_imager(fd);
        status = MISC_STATUS_IMG_SET;

        if (!fd) {
            break;
        }
    }
    return 0;
}

int update_path(void)
{
    int update_flag = 0;

    UPD_LOGI("start to upgrade");
    update_flag = update_process();
    if (!update_flag) {
        UPD_LOGI("suc update ^_^");
    } else {
        UPD_LOGE("e process");
        if (update_flag == UPDATE_PATCH_FAIL) {
            return update_flag;
        }
    }

    return misc_reset();
}

int update_init(void)
{
    int ret;
    uint32_t img_sector;
    uint32_t misc_start;
    uint32_t misc_size;
    uint32_t end_img_misc_addr;
    partition_t partition;
    partition_info_t *part_info;

    partition = partition_open(MTB_MISC_NAME);
    part_info = partition_info_get(partition);
    if (part_info == NULL) {
        UPD_LOGE("there is no misc partition.");
        return -1;
    }

    img_sector = part_info->sector_size;
#ifdef CONFIG_IMG_SECTOR_SIZE
    img_sector = CONFIG_IMG_SECTOR_SIZE;
#endif
    if (part_info != NULL) {
        misc_start = part_info->start_addr + part_info->base_addr;
        misc_size = part_info->length;
        end_img_misc_addr = misc_start + misc_size;
        UPD_LOGD("misc_start addr:0x%x, end_img_misc_addr:0x%x", misc_start, end_img_misc_addr);
        UPD_LOGD("sector_size:0x%x", part_info->sector_size);
        (void)img_sector;
        (void)end_img_misc_addr;
        ret = misc_init(misc_start, misc_size, part_info->sector_size);
        if (ret != 0) {
            UPD_LOGE("misc init e.");
            return ret;
        }
        ret = misc_file_check();
        UPD_LOGD("misc check ret:%d", ret);
        if (ret != -1) {
            UPD_LOGD("update mode");
            if (update_path()) {
                UPD_LOGE("e update");
            }
            boot_sys_reboot();
        } else {
            UPD_LOGW("e misc file");
        }        
    } else {
        UPD_LOGW("no misc partition.");
    }
    partition_close(partition);

    return 0;
}
#endif