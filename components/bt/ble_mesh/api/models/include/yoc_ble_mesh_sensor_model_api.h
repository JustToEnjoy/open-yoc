// Copyright 2017-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/** @file
 *  @brief Bluetooth Mesh Sensor Client Model APIs.
 */

#ifndef _BLE_MESH_BLE_MESH_SENSOR_MODEL_API_H_
#define _BLE_MESH_BLE_MESH_SENSOR_MODEL_API_H_

#include "sensor_client.h"
#include "yoc_ble_mesh_defs.h"

/** @def    BLE_MESH_BLE_MESH_MODEL_SENSOR_CLI
 *
 *  @brief  Define a new Sensor Client Model.
 *
 *  @note   This API needs to be called for each element on which
 *          the application needs to have a Sensor Client Model.
 *
 *  @param  cli_pub  Pointer to the unique struct ble_mesh_model_pub_t.
 *  @param  cli_data Pointer to the unique struct ble_mesh_client_t.
 *
 *  @return New Sensor Client Model instance.
 */
#define BLE_MESH_BLE_MESH_MODEL_SENSOR_CLI(cli_pub, cli_data)            \
        BLE_MESH_BLE_MESH_SIG_MODEL(BLE_MESH_BLE_MESH_MODEL_ID_SENSOR_CLI,    \
                    NULL, cli_pub, cli_data)

/**
 *  @brief Bluetooth Mesh Sensor Client Model Get and Set parameters structure.
 */

/** Parameters of Sensor Descriptor Get */
typedef struct {
    bool  op_en;        /*!< Indicate if optional parameters are included */
    u16_t property_id;  /*!< Property ID of a sensor (optional) */
} ble_mesh_sensor_descriptor_get_t;

/** Parameter of Sensor Cadence Get */
typedef struct {
    u16_t property_id;  /*!< Property ID of a sensor */
} ble_mesh_sensor_cadence_get_t;

/** Parameters of Sensor Cadence Set */
typedef struct {
    u16_t property_id;                                  /*!< Property ID for the sensor */
    u8_t  fast_cadence_period_divisor : 7,              /*!< Divisor for the publish period */
          status_trigger_type : 1;                      /*!< The unit and format of the Status Trigger Delta fields */
    struct net_buf_simple *status_trigger_delta_down;   /*!< Delta down value that triggers a status message */
    struct net_buf_simple *status_trigger_delta_up;     /*!< Delta up value that triggers a status message */
    u8_t  status_min_interval;                          /*!< Minimum interval between two consecutive Status messages */
    struct net_buf_simple *fast_cadence_low;            /*!< Low value for the fast cadence range */
    struct net_buf_simple *fast_cadence_high;           /*!< Fast value for the fast cadence range */
} ble_mesh_sensor_cadence_set_t;

/** Parameter of Sensor Settings Get */
typedef struct {
    u16_t sensor_property_id;   /*!< Property ID of a sensor */
} ble_mesh_sensor_settings_get_t;

/** Parameters of Sensor Setting Get */
typedef struct {
    u16_t sensor_property_id;           /*!< Property ID of a sensor */
    u16_t sensor_setting_property_id;   /*!< Setting ID identifying a setting within a sensor */
} ble_mesh_sensor_setting_get_t;

/** Parameters of Sensor Setting Set */
typedef struct {
    u16_t sensor_property_id;           /*!< Property ID identifying a sensor */
    u16_t sensor_setting_property_id;   /*!< Setting ID identifying a setting within a sensor */
    struct net_buf_simple *sensor_setting_raw;  /*!< Raw value for the setting */
} ble_mesh_sensor_setting_set_t;

/** Parameters of Sensor Get */
typedef struct {
    bool  op_en;        /*!< Indicate if optional parameters are included  */
    u16_t property_id;  /*!< Property ID for the sensor (optional) */
} ble_mesh_sensor_get_t;

/** Parameters of Sensor Column Get */
typedef struct {
    u16_t property_id;  /*!< Property identifying a sensor */
    struct net_buf_simple *raw_value_x; /*!< Raw value identifying a column */
} ble_mesh_sensor_column_get_t;

/** Parameters of Sensor Series Get */
typedef struct {
    bool  op_en;        /*!< Indicate if optional parameters are included */
    u16_t property_id;  /*!< Property identifying a sensor */
    struct net_buf_simple *raw_value_x1;    /*!< Raw value identifying a starting column (optional) */
    struct net_buf_simple *raw_value_x2;    /*!< Raw value identifying an ending column (C.1) */
} ble_mesh_sensor_series_get_t;

/**
 * @brief Sensor Client Model get message union
 */
typedef union {
    ble_mesh_sensor_descriptor_get_t descriptor_get;    /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET */
    ble_mesh_sensor_cadence_get_t    cadence_get;       /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_CADENCE_GET */
    ble_mesh_sensor_settings_get_t   settings_get;      /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_GET */
    ble_mesh_sensor_setting_get_t    setting_get;       /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_SETTING_GET */
    ble_mesh_sensor_get_t            sensor_get;        /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_GET */
    ble_mesh_sensor_column_get_t     column_get;        /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_COLUMN_GET */
    ble_mesh_sensor_series_get_t     series_get;        /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_SERIES_GET */
} ble_mesh_sensor_client_get_state_t;

/**
 * @brief Sensor Client Model set message union
 */
typedef union {
    ble_mesh_sensor_cadence_set_t cadence_set;          /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET & BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK */
    ble_mesh_sensor_setting_set_t setting_set;          /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_SETTING_SET & BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK */
} ble_mesh_sensor_client_set_state_t;

/**
 *  @brief Bluetooth Mesh Sensor Client Model Get and Set callback parameters structure.
 */

/** Parameter of Sensor Descriptor Status */
typedef struct {
    struct net_buf_simple *descriptor;  /*!< Sequence of 8-octet sensor descriptors (optional) */
} ble_mesh_sensor_descriptor_status_cb_t;

/** Parameters of Sensor Cadence Status */
typedef struct {
    u16_t property_id;  /*!< Property for the sensor */
    struct net_buf_simple *sensor_cadence_value;    /*!< Value of sensor cadence state */
} ble_mesh_sensor_cadence_status_cb_t;

/** Parameters of Sensor Settings Status */
typedef struct {
    u16_t sensor_property_id;   /*!< Property ID identifying a sensor */
    struct net_buf_simple *sensor_setting_property_ids; /*!< A sequence of N sensor setting property IDs (optional) */
} ble_mesh_sensor_settings_status_cb_t;

/** Parameters of Sensor Setting Status */
typedef struct  {
    bool  op_en;                        /*!< Indicate id optional parameters are included */
    u16_t sensor_property_id;           /*!< Property ID identifying a sensor */
    u16_t sensor_setting_property_id;   /*!< Setting ID identifying a setting within a sensor */
    u8_t  sensor_setting_access;        /*!< Read/Write access rights for the setting (optional) */
    struct net_buf_simple *sensor_setting_raw;  /*!< Raw value for the setting */
} ble_mesh_sensor_setting_status_cb_t;

/** Parameter of Sensor Status */
typedef struct {
    struct net_buf_simple *marshalled_sensor_data;  /*!< Value of sensor data state (optional) */
} ble_mesh_sensor_status_cb_t;

/** Parameters of Sensor Column Status */
typedef struct {
    u16_t property_id;  /*!< Property identifying a sensor and the Y axis  */
    struct net_buf_simple *sensor_column_value; /*!< Left values of sensor column status */
} ble_mesh_sensor_column_status_cb_t;

/** Parameters of Sensor Series Status */
typedef struct {
    u16_t property_id;  /*!< Property identifying a sensor and the Y axis  */
    struct net_buf_simple *sensor_series_value; /*!< Left values of sensor series status */
} ble_mesh_sensor_series_status_cb_t;

/**
 * @brief Sensor Client Model received message union
 */
typedef union {
    ble_mesh_sensor_descriptor_status_cb_t descriptor_status;   /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS */
    ble_mesh_sensor_cadence_status_cb_t    cadence_status;      /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_CADENCE_STATUS */
    ble_mesh_sensor_settings_status_cb_t   settings_status;     /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_STATUS */
    ble_mesh_sensor_setting_status_cb_t    setting_status;      /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_SETTING_STATUS */
    ble_mesh_sensor_status_cb_t            sensor_status;       /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_STATUS */
    ble_mesh_sensor_column_status_cb_t     column_status;       /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_COLUMN_STATUS */
    ble_mesh_sensor_series_status_cb_t     series_status;       /*!< For BLE_MESH_BLE_MESH_MODEL_OP_SENSOR_SERIES_STATUS */
} ble_mesh_sensor_client_status_cb_t;

/** Sensor Client Model callback parameters */
typedef struct {
    int error_code;     /*!< 0: success,
                         *   otherwise failure. For the error code values please refer to errno.h file.
                         *   A negative sign is added to the standard error codes in errno.h. */
    ble_mesh_client_common_param_t    *params;      /*!< The client common parameters. */
    ble_mesh_sensor_client_status_cb_t status_cb;   /*!< The sensor status message callback values */
} ble_mesh_sensor_client_cb_param_t;

/** This enum value is the event of Sensor Client Model */
typedef enum {
    BLE_MESH_BLE_MESH_SENSOR_CLIENT_GET_STATE_EVT,
    BLE_MESH_BLE_MESH_SENSOR_CLIENT_SET_STATE_EVT,
    BLE_MESH_BLE_MESH_SENSOR_CLIENT_PUBLISH_EVT,
    BLE_MESH_BLE_MESH_SENSOR_CLIENT_TIMEOUT_EVT,
    BLE_MESH_BLE_MESH_SENSOR_CLIENT_EVT_MAX,
} ble_mesh_sensor_client_cb_event_t;

/**
 *  @brief Bluetooth Mesh Sensor Client Model function.
 */

/**
 * @brief   Sensor Client Model callback function type
 * @param   event: Event type
 * @param   param: Pointer to callback parameter
 */
typedef void (* ble_mesh_sensor_client_cb_t)(ble_mesh_sensor_client_cb_event_t event,
        ble_mesh_sensor_client_cb_param_t *param);

/**
 * @brief       Register BLE Mesh Sensor Client Model callback.
 *
 * @param[in]   callback: Pointer to the callback function.
 *
 * @return      BT_OK on success or error code otherwise.
 *
 */
bt_err_t ble_mesh_register_sensor_client_callback(ble_mesh_sensor_client_cb_t callback);

/**
 * @brief       Get the value of Sensor Server Model states using the Sensor Client Model get messages.
 *
 * @note        If you want to know the opcodes and corresponding meanings accepted by this API,
 *              please refer to ble_mesh_sensor_message_opcode_t in yoc_ble_mesh_defs.h
 *
 * @param[in]   params:    Pointer to BLE Mesh common client parameters.
 * @param[in]   get_state: Pointer to sensor get message value.
 *                         Shall not be set to NULL.
 *
 * @return      BT_OK on success or error code otherwise.
 *
 */
bt_err_t ble_mesh_sensor_client_get_state(ble_mesh_client_common_param_t *params,
        ble_mesh_sensor_client_get_state_t *get_state);

/**
 * @brief       Set the value of Sensor Server Model states using the Sensor Client Model set messages.
 *
 * @note        If you want to know the opcodes and corresponding meanings accepted by this API,
 *              please refer to ble_mesh_sensor_message_opcode_t in yoc_ble_mesh_defs.h
 *
 * @param[in]   params:    Pointer to BLE Mesh common client parameters.
 * @param[in]   set_state: Pointer to sensor set message value.
 *                         Shall not be set to NULL.
 *
 * @return      BT_OK on success or error code otherwise.
 *
 */
bt_err_t ble_mesh_sensor_client_set_state(ble_mesh_client_common_param_t *params,
        ble_mesh_sensor_client_set_state_t *set_state);

#endif /* _BLE_MESH_BLE_MESH_SENSOR_MODEL_API_H_ */


