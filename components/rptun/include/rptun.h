/*
 * Copyright (C) 2020 FishSemi Inc. All rights reserved.
 */

/******************************************************************************
 * @file     rptun.h
 * @brief    head file for rptun
 * @version  V1.0
 * @date     20. Jan 2020
 ******************************************************************************/

#ifndef _RPTUN_H_
#define _RPTUN_H_

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openamp/open_amp.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define RPTUNIOC_START              1
#define RPTUNIOC_STOP               2

#define RPTUN_NOTIFY_ALL            (0xffffffff - 0)

/* Access macros ************************************************************/

/****************************************************************************
 * Name: RPTUN_GET_CPUNAME
 *
 * Description:
 *   Get remote cpu name
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *
 * Returned Value:
 *   Cpu name on success, NULL on failure.
 *
 ****************************************************************************/

#define RPTUN_GET_CPUNAME(d) ((d)->ops->get_cpuname(d))

/****************************************************************************
 * Name: RPTUN_GET_FIRMWARE
 *
 * Description:
 *   Get remote firmware name
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *
 * Returned Value:
 *   Firmware name on success, NULL on failure.
 *
 ****************************************************************************/

#define RPTUN_GET_FIRMWARE(d) ((d)->ops->get_firmware(d))

/****************************************************************************
 * Name: RPTUN_GET_ADDRENV
 *
 * Description:
 *   Get adress env list
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *
 * Returned Value:
 *   Addrenv pointer on success, NULL on failure.
 *
 ****************************************************************************/

#define RPTUN_GET_ADDRENV(d) ((d)->ops->get_addrenv(d))

/****************************************************************************
 * Name: RPTUN_GET_RESOURCE
 *
 * Description:
 *   Get rptun resouce
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *
 * Returned Value:
 *   Resource pointer on success, NULL on failure
 *
 ****************************************************************************/

#define RPTUN_GET_RESOURCE(d) ((d)->ops->get_resource(d))

/****************************************************************************
 * Name: RPTUN_IS_AUTOSTART
 *
 * Description:
 *   AUTO start or not
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *
 * Returned Value:
 *   True autostart, false not autostart
 *
 ****************************************************************************/

#define RPTUN_IS_AUTOSTART(d) ((d)->ops->is_autostart(d))

/****************************************************************************
 * Name: RPTUN_IS_MASTER
 *
 * Description:
 *   IS master or not
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *
 * Returned Value:
 *   True master, false remote
 *
 ****************************************************************************/

#define RPTUN_IS_MASTER(d) ((d)->ops->is_master(d))

/****************************************************************************
 * Name: RPTUN_CONFIG
 *
 * Description:
 *   CONFIG remote cpu
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   data - Device-specific private data
 *
 * Returned Value:
 *   OK unless an error occurs.  Then a negated errno value is returned
 *
 ****************************************************************************/
#define RPTUN_CONFIG(d, p) ((d)->ops->config(d, p))

/****************************************************************************
 * Name: RPTUN_START
 *
 * Description:
 *   START remote cpu
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *
 * Returned Value:
 *   OK unless an error occurs.  Then a negated errno value is returned
 *
 ****************************************************************************/

#define RPTUN_START(d) ((d)->ops->start(d))

/****************************************************************************
 * Name: RPTUN_STOP
 *
 * Description:
 *   STOP remote cpu
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *
 * Returned Value:
 *   OK unless an error occurs.  Then a negated errno value is returned
 *
 ****************************************************************************/

#define RPTUN_STOP(d) ((d)->ops->stop(d))

/****************************************************************************
 * Name: RPTUN_NOTIFY
 *
 * Description:
 *   Notify remote core there is a message to get.
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   vqid - Message to notify
 *
 * Returned Value:
 *   OK unless an error occurs.  Then a negated errno value is returned
 *
 ****************************************************************************/

#define RPTUN_NOTIFY(d,v) ((d)->ops->notify(d,v))

/****************************************************************************
 * Name: RPTUN_REGISTER_CALLBACK
 *
 * Description:
 *   Attach to receive a callback when something is received on RPTUN
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   callback - The function to be called when something has been received
 *   arg      - A caller provided value to return with the callback
 *
 * Returned Value:
 *   OK unless an error occurs.  Then a negated errno value is returned
 *
 ****************************************************************************/

#define RPTUN_REGISTER_CALLBACK(d,c,a) ((d)->ops->register_callback(d,c,a))

/****************************************************************************
 * Name: RPTUN_UNREGISTER_CALLBACK
 *
 * Description:
 *   Detach RPTUN callback
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *
 * Returned Value:
 *   OK unless an error occurs.  Then a negated errno value is returned
 *
 ****************************************************************************/

#define RPTUN_UNREGISTER_CALLBACK(d) ((d)->ops->register_callback(d,NULL,NULL))

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef int (*rptun_callback_t)(void *arg, uint32_t vqid);

struct rptun_addrenv_s
{
    uintptr_t pa;
    uintptr_t da;
    size_t    size;
};

struct __attribute__((aligned(B2C(8)))) rptun_rsc_s
{
    struct resource_table    rsc_tbl_hdr;
    unsigned int             offset[2];
    struct fw_rsc_trace      log_trace;
    struct fw_rsc_vdev       rpmsg_vdev;
    struct fw_rsc_vdev_vring rpmsg_vring0;
    struct fw_rsc_vdev_vring rpmsg_vring1;
    unsigned int             buf_size;
};

struct rptun_dev_s;
struct rptun_ops_s
{
    const char *(*get_cpuname)(struct rptun_dev_s *dev);
    const char *(*get_firmware)(struct rptun_dev_s *dev);

    const struct rptun_addrenv_s *(*get_addrenv)(struct rptun_dev_s *dev);
    struct rptun_rsc_s *(*get_resource)(struct rptun_dev_s *dev);

    bool (*is_autostart)(struct rptun_dev_s *dev);
    bool (*is_master)(struct rptun_dev_s *dev);

    int (*config)(struct rptun_dev_s *dev, void *data);
    int (*start)(struct rptun_dev_s *dev);
    int (*stop)(struct rptun_dev_s *dev);
    int (*notify)(struct rptun_dev_s *dev, uint32_t vqid);
    int (*register_callback)(struct rptun_dev_s *dev,
                    rptun_callback_t callback, void *arg);
};

struct rptun_dev_s
{
    const struct rptun_ops_s *ops;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

int rptun_initialize(struct rptun_dev_s *dev);
int rptun_boot(const char *cpuname);

#ifdef __cplusplus
}
#endif
#endif /* _RPTUN_H_ */