/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */

#ifndef _SAL_H_
#define _SAL_H_

#include <stdint.h>
#include <aos/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SAL_PACKET_SEND_MODE_ASYNC   0
//#define SAL_SERVER                   1

#ifndef CONFIG_SAL_DEFAULT_INPUTMBOX_SIZE        
#define CONFIG_SAL_DEFAULT_INPUTMBOX_SIZE        16
#endif

#ifndef CONFIG_SAL_DEFAULT_OUTPUTMBOX_SIZE       
#define CONFIG_SAL_DEFAULT_OUTPUTMBOX_SIZE       8
#endif

typedef enum {
    /* WiFi */
    TCP_SERVER,
    TCP_CLIENT,
    SSL_CLIENT,
    UDP_BROADCAST,
    UDP_UNICAST,
    /*WiFi end */
    /* Add others hereafter */
} CONN_TYPE;

/* Fill necessary fileds according to the socket type. */
typedef struct {
    int fd; /* fd that are used in socket level */
    CONN_TYPE type;
    char *addr; /* remote ip or domain */
    int32_t r_port; /* remote port (set to -1 if not used) */
    int32_t l_port; /* local port (set to -1 if not used) */
    uint32_t tcp_keep_alive; /* tcp keep alive value (set to 0 if not used) */
} sal_conn_t;

#ifdef SAL_SERVER
typedef enum {
    CLIENT_NONE,
    CLIENT_CONNECTED,
    CLIENT_CLOSED,
} client_status_t;

typedef int (*netconn_client_status_notify_t)(int fd, client_status_t status, char remote_ip[16], uint16_t remote_port);
#endif

typedef int (*netconn_data_input_cb_t)(int fd, void *data, size_t len, char remote_ip[16], uint16_t remote_port);
typedef int (*netconn_close_cb_t)(int fd);

typedef struct sal_op_s {
    char *version; /* Reserved for furture use. */

    /**
     * Module low level init so that it's ready to setup socket connection.
     *
     * @return  0 - success, -1 - failure
     */
    int (*init)(void);

    /**
     * Start a socket connection via module.
     *
     * @param[in]  c - connect parameters which are used to setup
     *                 the socket connection.
     *
     * @return  0 - success, -1 - failure
     */
    int (*start)(sal_conn_t *c);

    /**
     * Send data via module.
     * This function does not return until all data sent.
     *
     * @param[in]  fd - the file descripter to operate on.
     * @param[in]  data - pointer to data to send.
     * @param[in]  len - length of the data.
     * @param[in]  remote_ip - remote ip address (optional).
     * @param[in]  remote_port - remote port number (optional).
     * @param[in]  timeout - packet send timeout (ms)
     * @return  0 - success, -1 - failure
     */
    int (*send)(int fd, uint8_t *data, uint32_t len,
                char remote_ip[16], int32_t remote_port, int32_t timeout);

    int (*recv)(int fd, uint8_t *data, uint32_t len,
                char remote_ip[16], int32_t remote_port);

    /**
     * Get IP information of the corresponding domain.
     * Currently only one IP string is returned (even when the domain
     * coresponses to mutliple IPs). Note: only IPv4 is supported.
     *
     * @param[in]   domain - the domain string.
     * @param[out]  ip - the place to hold the dot-formatted ip string.
     *
     * @return  0 - success, -1 - failure
     */
    int (*domain_to_ip)(char *domain, char ip[16]);

    /**
     * Close the socket connection.
     *
     * @param[in]  fd - the file descripter to operate on.
     * @param[in]  remote_port - remote port number (optional).
     *
     * @return  0 - success, -1 - failure
     */
    int (*close)(int fd, int32_t remote_port);

    /**
     * Destroy SAL or exit low level state if necessary.
     *
     * @return  0 - success, -1 - failure
     */
    int (*deinit)(void);

    /**
     * Register network connection data input function
     * Input data from module.
     * This callback should be called when the data is received from the module
     * It should tell the sal where the data comes from.
     * @param[in]  fd - the file descripter to operate on.
     * @param[in]  data - the received data.
     * @param[in]  len - expected length of the data when IN,
     *                    and real read len when OUT.
     * @param[in]  addr - remote ip address. Caller manages the
                                memory (optional).
     * @param[in]  port - remote port number (optional).
     *
     * @return  0 - success, -1 - failure
     */
    int (*register_netconn_data_input_cb)(netconn_data_input_cb_t cb);

    int (*register_netconn_close_cb)(netconn_close_cb_t cb);

#ifdef SAL_SERVER
    /**
    * Register remote client status function
    * Input data from module.
    * This callback should be called when the data is received from the module
    * It should tell the sal where the data comes from.
    * @param[in]  fd - the file descripter to operate on.
    * @param[in]  status - remote client status
    * @param[in]  addr - remote ip address. Caller manages the
                              memory (optional).
    * @param[in]  port - remote port number (optional).
    *
    * @return  0 - success, -1 - failure
    */
    int (*register_netconn_client_status_notify)(netconn_client_status_notify_t cb);
#endif
} sal_op_t;


/**
 * SAL init(deinit)
 *
**/
int sal_init(void);
int sal_deinit(void);

/**
 * Register a module instance to the SAL
 *
 * @param[in] module the module instance
**/
int sal_module_register(sal_op_t *module);

/**
 * Unregister a module instance to the SAL
 *
**/
int sal_module_unregister(void);

/**
 * Module low level init so that it's ready to setup socket connection.
 *
 * @return  0 - success, -1 - failure
 */
int sal_module_init(void);

/**
 * Start a socket connection via module.
 *
 * @param[in]  conn - connect parameters which are used to setup
 *                 the socket connection.
 *
 * @return  0 - success, -1 - failure
 */
int sal_module_start(sal_conn_t *conn);

/**
 * Send data via module.
 * This function does not return until all data sent.
 *
 * @param[in]  fd - the file descripter to operate on.
 * @param[in]  data - pointer to data to send.
 * @param[in]  len - length of the data.
 * @param[in]  remote_ip - remote port number (optional).
 * @param[in]  remote_port - remote port number (optional).
 *
 * @return  0 - success, -1 - failure
 */
int sal_module_send(int fd, uint8_t *data, uint32_t len, char remote_ip[16],
                    int32_t remote_port, int32_t timeout);

/**
 * Get IP information of the corresponding domain.
 * Currently only one IP string is returned (even when the domain
 * coresponses to mutliple IPs). Note: only IPv4 is supported.
 *
 * @param[in]   domain - the domain string.
 * @param[out]  ip - the place to hold the dot-formatted ip string.
 *
 * @return  0 - success, -1 - failure
 */
int sal_module_domain_to_ip(char *domain, char ip[16]);

/**
 * Close the socket connection.
 *
 * @param[in]  fd - the file descripter to operate on.
 * @param[in]  remote_port - remote port number (optional).
 *
 * @return  0 - success, -1 - failure
 */
int sal_module_close(int fd, int32_t remote_port);

/**
 * Destroy SAL or exit low level state if necessary.
 *
 * @return  0 - success, -1 - failure
 */
int sal_module_deinit(void);

/**
 * Register network connection data input function
 * Input data from module.
 * This callback should be called when the data is received from the module
 * It should tell the sal where the data comes from.
 * @param[in]  fd - the file descripter to operate on.
 * @param[in]  data - the received data.
 * @param[in]  len - expected length of the data when IN,
 *                    and real read len when OUT.
 * @param[in]  addr - remote ip address. Caller manages the
                            memory (optional).
 * @param[in]  port - remote port number (optional).
 *
 * @return  0 - success, -1 - failure
 */
int sal_module_register_netconn_data_input_cb(netconn_data_input_cb_t cb);

int sal_module_register_netconn_close_cb(netconn_close_cb_t cb);

#ifdef SAL_SERVER
/**
 * Register remote client status function
 * Input data from module.
 * This callback should be called when the data is received from the module
 * It should tell the sal where the data comes from.
 * @param[in]  fd - the file descripter to operate on.
 * @param[in]  status - remote client status
 * @param[in]  addr - remote ip address. Caller manages the
                            memory (optional).
 * @param[in]  port - remote port number (optional).
 *
 * @return  0 - success, -1 - failure
 */
int sal_module_register_client_status_notify_cb(netconn_client_status_notify_t cb);
#endif

#ifdef __cplusplus
}
#endif

#endif
