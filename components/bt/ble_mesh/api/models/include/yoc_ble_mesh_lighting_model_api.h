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
 *  @brief Bluetooth Mesh Light Client Model APIs.
 */

#ifndef _BLE_MESH_BLE_MESH_LIGHTING_MODEL_API_H_
#define _BLE_MESH_BLE_MESH_LIGHTING_MODEL_API_H_

#include "lighting_client.h"
#include "yoc_ble_mesh_defs.h"

/** @def    BLE_MESH_BLE_MESH_MODEL_LIGHT_LIGHTNESS_CLI
 *
 *  @brief  Define a new Light Lightness Client Model.
 *
 *  @note   This API needs to be called for each element on which
 *          the application needs to have a Light Lightness Client Model.
 *
 *  @param  cli_pub  Pointer to the unique struct ble_mesh_model_pub_t.
 *  @param  cli_data Pointer to the unique struct ble_mesh_client_t.
 *
 *  @return New Light Lightness Client Model instance.
 */
#define BLE_MESH_BLE_MESH_MODEL_LIGHT_LIGHTNESS_CLI(cli_pub, cli_data)           \
        BLE_MESH_BLE_MESH_SIG_MODEL(BLE_MESH_BLE_MESH_MODEL_ID_LIGHT_LIGHTNESS_CLI,   \
                    NULL, cli_pub, cli_data)

/** @def    BLE_MESH_BLE_MESH_MODEL_LIGHT_CTL_CLI
 *
 *  @brief  Define a new Light CTL Client Model.
 *
 *  @note   This API needs to be called for each element on which
 *          the application needs to have a Light CTL Client Model.
 *
 *  @param  cli_pub  Pointer to the unique struct ble_mesh_model_pub_t.
 *  @param  cli_data Pointer to the unique struct ble_mesh_client_t.
 *
 *  @return New Light CTL Client Model instance.
 */
#define BLE_MESH_BLE_MESH_MODEL_LIGHT_CTL_CLI(cli_pub, cli_data)             \
        BLE_MESH_BLE_MESH_SIG_MODEL(BLE_MESH_BLE_MESH_MODEL_ID_LIGHT_CTL_CLI,     \
                    NULL, cli_pub, cli_data)

/** @def    BLE_MESH_BLE_MESH_MODEL_LIGHT_HSL_CLI
 *
 *  @brief  Define a new Light HSL Client Model.
 *
 *  @note   This API needs to be called for each element on which
 *          the application needs to have a Light HSL Client Model.
 *
 *  @param  cli_pub  Pointer to the unique struct ble_mesh_model_pub_t.
 *  @param  cli_data Pointer to the unique struct ble_mesh_client_t.
 *
 *  @return New Light HSL Client Model instance.
 */
#define BLE_MESH_BLE_MESH_MODEL_LIGHT_HSL_CLI(cli_pub, cli_data)             \
        BLE_MESH_BLE_MESH_SIG_MODEL(BLE_MESH_BLE_MESH_MODEL_ID_LIGHT_HSL_CLI,     \
                    NULL, cli_pub, cli_data)

/** @def    BLE_MESH_BLE_MESH_MODEL_LIGHT_XYL_CLI
 *
 *  @brief  Define a new Light xyL Client Model.
 *
 *  @note   This API needs to be called for each element on which
 *          the application needs to have a Light xyL Client Model.
 *
 *  @param  cli_pub  Pointer to the unique struct ble_mesh_model_pub_t.
 *  @param  cli_data Pointer to the unique struct ble_mesh_client_t.
 *
 *  @return New Light xyL Client Model instance.
 */
#define BLE_MESH_BLE_MESH_MODEL_LIGHT_XYL_CLI(cli_pub, cli_data)             \
        BLE_MESH_BLE_MESH_SIG_MODEL(BLE_MESH_BLE_MESH_MODEL_ID_LIGHT_XYL_CLI,     \
                    NULL, cli_pub, cli_data)

/** @def    BLE_MESH_BLE_MESH_MODEL_LIGHT_LC_CLI
 *
 *  @brief  Define a new Light LC Client Model.
 *
 *  @note   This API needs to be called for each element on which
 *          the application needs to have a Light LC Client Model.
 *
 *  @param  cli_pub  Pointer to the unique struct ble_mesh_model_pub_t.
 *  @param  cli_data Pointer to the unique struct ble_mesh_client_t.
 *
 *  @return New Light LC Client Model instance.
 */
#define BLE_MESH_BLE_MESH_MODEL_LIGHT_LC_CLI(cli_pub, cli_data)             \
        BLE_MESH_BLE_MESH_SIG_MODEL(BLE_MESH_BLE_MESH_MODEL_ID_LIGHT_LC_CLI,     \
                    NULL, cli_pub, cli_data)

/**
 *  @brief Bluetooth Mesh Light Lightness Client Model Get and Set parameters structure.
 */

/** Parameters of Light Lightness Set */
typedef struct {
    bool  op_en;        /*!< Indicate if optional parameters are included */
    u16_t lightness;    /*!< Target value of light lightness actual state */
    u8_t  tid;          /*!< Transaction ID */
    u8_t  trans_time;   /*!< Time to complete state transition (optional) */
    u8_t  delay;        /*!< Indicate message execution delay (C.1) */
} ble_mesh_light_lightness_set_t;

/** Parameters of Light Lightness Linear Set */
typedef struct {
    bool  op_en;        /*!< Indicate if optional parameters are included */
    u16_t lightness;    /*!< Target value of light lightness linear state */
    u8_t  tid;          /*!< Transaction ID */
    u8_t  trans_time;   /*!< Time to complete state transition (optional) */
    u8_t  delay;        /*!< Indicate message execution delay (C.1) */
} ble_mesh_light_lightness_linear_set_t;

/** Parameter of Light Lightness Default Set */
typedef struct {
    u16_t lightness;    /*!< The value of the Light Lightness Default state */
} ble_mesh_light_lightness_default_set_t;

/** Parameters of Light Lightness Range Set */
typedef struct {
    u16_t range_min;    /*!< Value of range min field of light lightness range state */
    u16_t range_max;    /*!< Value of range max field of light lightness range state */
} ble_mesh_light_lightness_range_set_t;

/** Parameters of Light CTL Set */
typedef struct {
    bool  op_en;            /*!< Indicate if optional parameters are included */
    u16_t ctl_lightness;    /*!< Target value of light ctl lightness state */
    u16_t ctl_temperatrue;  /*!< Target value of light ctl temperature state */
    s16_t ctl_delta_uv;     /*!< Target value of light ctl delta UV state */
    u8_t  tid;              /*!< Transaction ID */
    u8_t  trans_time;       /*!< Time to complete state transition (optional) */
    u8_t  delay;            /*!< Indicate message execution delay (C.1) */
} ble_mesh_light_ctl_set_t;

/** Parameters of Light CTL Temperature Set */
typedef struct {
    bool  op_en;            /*!< Indicate if optional parameters are included */
    u16_t ctl_temperatrue;  /*!< Target value of light ctl temperature state */
    s16_t ctl_delta_uv;     /*!< Target value of light ctl delta UV state */
    u8_t  tid;              /*!< Transaction ID */
    u8_t  trans_time;       /*!< Time to complete state transition (optional) */
    u8_t  delay;            /*!< Indicate message execution delay (C.1) */
} ble_mesh_light_ctl_temperature_set_t;

/** Parameters of Light CTL Temperature Range Set */
typedef struct {
    u16_t range_min;        /*!< Value of temperature range min field of light ctl temperature range state */
    u16_t range_max;        /*!< Value of temperature range max field of light ctl temperature range state */
} ble_mesh_light_ctl_temperature_range_set_t;

/** Parameters of Light CTL Default Set */
typedef struct {
    u16_t lightness;        /*!< Value of light lightness default state */
    u16_t temperature;      /*!< Value of light temperature default state */
    s16_t delta_uv;         /*!< Value of light delta UV default state */
} ble_mesh_light_ctl_default_set_t;

/** Parameters of Light HSL Set */
typedef struct {
    bool  op_en;            /*!< Indicate if optional parameters are included */
    u16_t hsl_lightness;    /*!< Target value of light hsl lightness state */
    u16_t hsl_hue;          /*!< Target value of light hsl hue state */
    u16_t hsl_saturation;   /*!< Target value of light hsl saturation state */
    u8_t  tid;              /*!< Transaction ID */
    u8_t  trans_time;       /*!< Time to complete state transition (optional) */
    u8_t  delay;            /*!< Indicate message execution delay (C.1) */
} ble_mesh_light_hsl_set_t;

/** Parameters of Light HSL Hue Set */
typedef struct {
    bool  op_en;            /*!< Indicate if optional parameters are included */
    u16_t hue;              /*!< Target value of light hsl hue state */
    u8_t  tid;              /*!< Transaction ID */
    u8_t  trans_time;       /*!< Time to complete state transition (optional) */
    u8_t  delay;            /*!< Indicate message execution delay (C.1) */
} ble_mesh_light_hsl_hue_set_t;

/** Parameters of Light HSL Saturation Set */
typedef struct {
    bool  op_en;            /*!< Indicate if optional parameters are included */
    u16_t saturation;       /*!< Target value of light hsl hue state */
    u8_t  tid;              /*!< Transaction ID */
    u8_t  trans_time;       /*!< Time to complete state transition (optional) */
    u8_t  delay;            /*!< Indicate message execution delay (C.1) */
} ble_mesh_light_hsl_saturation_set_t;

/** Parameters of Light HSL Default Set */
typedef struct {
    u16_t lightness;        /*!< Value of light lightness default state */
    u16_t hue;              /*!< Value of light hue default state */
    u16_t saturation;       /*!< Value of light saturation default state */
} ble_mesh_light_hsl_default_set_t;

/** Parameters of Light HSL Range Set */
typedef struct {
    u16_t hue_range_min;        /*!< Value of hue range min field of light hsl hue range state */
    u16_t hue_range_max;        /*!< Value of hue range max field of light hsl hue range state */
    u16_t saturation_range_min; /*!< Value of saturation range min field of light hsl saturation range state */
    u16_t saturation_range_max; /*!< Value of saturation range max field of light hsl saturation range state */
} ble_mesh_light_hsl_range_set_t;

/** Parameters of Light xyL Set */
typedef struct {
    bool  op_en;            /*!< Indicate whether optional parameters included */
    u16_t xyl_lightness;    /*!< The target value of the Light xyL Lightness state */
    u16_t xyl_x;            /*!< The target value of the Light xyL x state */
    u16_t xyl_y;            /*!< The target value of the Light xyL y state */
    u8_t  tid;              /*!< Transaction Identifier */
    u8_t  trans_time;       /*!< Time to complete state transition (optional) */
    u8_t  delay;            /*!< Indicate message execution delay (C.1) */
} ble_mesh_light_xyl_set_t;

/** Parameters of Light xyL Default Set */
typedef struct {
    u16_t lightness;        /*!< The value of the Light Lightness Default state */
    u16_t xyl_x;            /*!< The value of the Light xyL x Default state */
    u16_t xyl_y;            /*!< The value of the Light xyL y Default state */
} ble_mesh_light_xyl_default_set_t;

/** Parameters of Light xyL Range Set */
typedef struct {
    u16_t xyl_x_range_min;  /*!< The value of the xyL x Range Min field of the Light xyL x Range state */
    u16_t xyl_x_range_max;  /*!< The value of the xyL x Range Max field of the Light xyL x Range state */
    u16_t xyl_y_range_min;  /*!< The value of the xyL y Range Min field of the Light xyL y Range state */
    u16_t xyl_y_range_max;  /*!< The value of the xyL y Range Max field of the Light xyL y Range state */
} ble_mesh_light_xyl_range_set_t;

/** Parameter of Light LC Mode Set */
typedef struct {
    u8_t mode;              /*!< The target value of the Light LC Mode state */
} ble_mesh_light_lc_mode_set_t;

/** Parameter of Light LC OM Set */
typedef struct {
    u8_t mode;              /*!< The target value of the Light LC Occupancy Mode state */
} ble_mesh_light_lc_om_set_t;

/** Parameters of Light LC Light OnOff Set */
typedef struct {
    bool op_en;             /*!< Indicate whether optional parameters included */
    u8_t light_onoff;       /*!< The target value of the Light LC Light OnOff state */
    u8_t tid;               /*!< Transaction Identifier */
    u8_t trans_time;        /*!< Time to complete state transition (optional) */
    u8_t delay;             /*!< Indicate message execution delay (C.1) */
} ble_mesh_light_lc_light_onoff_set_t;

/** Parameter of Light LC Property Get */
typedef struct {
    u16_t property_id;      /*!< Property ID identifying a Light LC Property */
} ble_mesh_light_lc_property_get_t;

/** Parameters of Light LC Property Set */
typedef struct {
    u16_t property_id;      /*!< Property ID identifying a Light LC Property */
    struct net_buf_simple *property_value;  /*!< Raw value for the Light LC Property */
} ble_mesh_light_lc_property_set_t;

/**
 * @brief Lighting Client Model get message union
 */
typedef union {
    ble_mesh_light_lc_property_get_t           lc_property_get;             /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_GET */
} ble_mesh_light_client_get_state_t;

/**
 * @brief Lighting Client Model set message union
 */
typedef union {
    ble_mesh_light_lightness_set_t             lightness_set;               /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_SET_UNACK */
    ble_mesh_light_lightness_linear_set_t      lightness_linear_set;        /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_SET_UNACK */
    ble_mesh_light_lightness_default_set_t     lightness_default_set;       /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK */
    ble_mesh_light_lightness_range_set_t       lightness_range_set;         /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_SET_UNACK */
    ble_mesh_light_ctl_set_t                   ctl_set;                     /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_SET_UNACK */
    ble_mesh_light_ctl_temperature_set_t       ctl_temperature_set;         /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_SET_UNACK */
    ble_mesh_light_ctl_temperature_range_set_t ctl_temperature_range_set;   /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK */
    ble_mesh_light_ctl_default_set_t           ctl_default_set;             /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_SET_UNACK */
    ble_mesh_light_hsl_set_t                   hsl_set;                     /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_SET_UNACK */
    ble_mesh_light_hsl_hue_set_t               hsl_hue_set;                 /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_SET_UNACK */
    ble_mesh_light_hsl_saturation_set_t        hsl_saturation_set;          /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_SET_UNACK */
    ble_mesh_light_hsl_default_set_t           hsl_default_set;             /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_SET_UNACK */
    ble_mesh_light_hsl_range_set_t             hsl_range_set;               /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_SET_UNACK */
    ble_mesh_light_xyl_set_t                   xyl_set;                     /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_SET_UNACK */
    ble_mesh_light_xyl_default_set_t           xyl_default_set;             /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_SET_UNACK */
    ble_mesh_light_xyl_range_set_t             xyl_range_set;               /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_SET_UNACK */
    ble_mesh_light_lc_mode_set_t               lc_mode_set;                 /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_SET_UNACK */
    ble_mesh_light_lc_om_set_t                 lc_om_set;                   /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_OM_SET_UNACK */
    ble_mesh_light_lc_light_onoff_set_t        lc_light_onoff_set;          /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_SET_UNACK */
    ble_mesh_light_lc_property_set_t           lc_property_set;             /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET & BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_SET_UNACK */
} ble_mesh_light_client_set_state_t;

/**
 *  @brief Bluetooth Mesh Light Lightness Client Model Get and Set callback parameters structure.
 */

/** Parameters of Light Lightness Status */
typedef struct {
    bool  op_en;                /*!< Indicate if optional parameters are included */
    u16_t present_lightness;    /*!< Current value of light lightness actual state */
    u16_t target_lightness;     /*!< Target value of light lightness actual state (optional) */
    u8_t  remain_time;          /*!< Time to complete state transition (C.1) */
} ble_mesh_light_lightness_status_cb_t;

/** Parameters of Light Lightness Linear Status */
typedef struct {
    bool  op_en;                /*!< Indicate if optional parameters are included */
    u16_t present_lightness;    /*!< Current value of light lightness linear state */
    u16_t target_lightness;     /*!< Target value of light lightness linear state (optional) */
    u8_t  remain_time;          /*!< Time to complete state transition (C.1) */
} ble_mesh_light_lightness_linear_status_cb_t;

/** Parameter of Light Lightness Last Status */
typedef struct {
    u16_t lightness;            /*!< The value of the Light Lightness Last state */
} ble_mesh_light_lightness_last_status_cb_t;

/** Parameter of Light Lightness Default Status */
typedef struct {
    u16_t lightness;            /*!< The value of the Light Lightness default State */
} ble_mesh_light_lightness_default_status_cb_t;

/** Parameters of Light Lightness Range Status */
typedef struct {
    u8_t  status_code;          /*!< Status Code for the request message */
    u16_t range_min;            /*!< Value of range min field of light lightness range state */
    u16_t range_max;            /*!< Value of range max field of light lightness range state */
} ble_mesh_light_lightness_range_status_cb_t;

/** Parameters of Light CTL Status */
typedef struct {
    bool  op_en;                    /*!< Indicate if optional parameters are included */
    u16_t present_ctl_lightness;    /*!< Current value of light ctl lightness state */
    u16_t present_ctl_temperature;  /*!< Current value of light ctl temperature state */
    u16_t target_ctl_lightness;     /*!< Target value of light ctl lightness state (optional) */
    u16_t target_ctl_temperature;   /*!< Target value of light ctl temperature state (C.1) */
    u8_t  remain_time;              /*!< Time to complete state transition (C.1) */
} ble_mesh_light_ctl_status_cb_t;

/** Parameters of Light CTL Temperature Status */
typedef struct {
    bool  op_en;                    /*!< Indicate if optional parameters are included */
    u16_t present_ctl_temperature;  /*!< Current value of light ctl temperature state */
    u16_t present_ctl_delta_uv;     /*!< Current value of light ctl delta UV state */
    u16_t target_ctl_temperature;   /*!< Target value of light ctl temperature state (optional) */
    u16_t target_ctl_delta_uv;      /*!< Target value of light ctl delta UV state (C.1) */
    u8_t  remain_time;              /*!< Time to complete state transition (C.1) */
} ble_mesh_light_ctl_temperature_status_cb_t;

/** Parameters of Light CTL Temperature Range Status */
typedef struct {
    u8_t  status_code;      /*!< Status code for the request message */
    u16_t range_min;        /*!< Value of temperature range min field of light ctl temperature range state */
    u16_t range_max;        /*!< Value of temperature range max field of light ctl temperature range state */
} ble_mesh_light_ctl_temperature_range_status_cb_t;

/** Parameters of Light CTL Default Status */
typedef struct {
    u16_t lightness;        /*!< Value of light lightness default state */
    u16_t temperature;      /*!< Value of light temperature default state */
    s16_t delta_uv;         /*!< Value of light delta UV default state */
} ble_mesh_light_ctl_default_status_cb_t;

/** Parameters of Light HSL Status */
typedef struct {
    bool  op_en;            /*!< Indicate if optional parameters are included */
    u16_t hsl_lightness;    /*!< Current value of light hsl lightness state */
    u16_t hsl_hue;          /*!< Current value of light hsl hue state */
    u16_t hsl_saturation;   /*!< Current value of light hsl saturation state */
    u8_t  remain_time;      /*!< Time to complete state transition (optional) */
} ble_mesh_light_hsl_status_cb_t;

/** Parameters of Light HSL Target Status */
typedef struct {
    bool  op_en;                    /*!< Indicate if optional parameters are included */
    u16_t hsl_lightness_target;     /*!< Target value of light hsl lightness state */
    u16_t hsl_hue_target;           /*!< Target value of light hsl hue state */
    u16_t hsl_saturation_target;    /*!< Target value of light hsl saturation state */
    u8_t  remain_time;              /*!< Time to complete state transition (optional) */
} ble_mesh_light_hsl_target_status_cb_t;

/** Parameters of Light HSL Hue Status */
typedef struct {
    bool  op_en;        /*!< Indicate if optional parameters are included */
    u16_t present_hue;  /*!< Current value of light hsl hue state */
    u16_t target_hue;   /*!< Target value of light hsl hue state (optional) */
    u8_t  remain_time;  /*!< Time to complete state transition (C.1) */
} ble_mesh_light_hsl_hue_status_cb_t;

/** Parameters of Light HSL Saturation Status */
typedef struct {
    bool  op_en;                /*!< Indicate if optional parameters are included */
    u16_t present_saturation;   /*!< Current value of light hsl saturation state */
    u16_t target_saturation;    /*!< Target value of light hsl saturation state (optional) */
    u8_t  remain_time;          /*!< Time to complete state transition (C.1) */
} ble_mesh_light_hsl_saturation_status_cb_t;

/** Parameters of Light HSL Default Status */
typedef struct {
    u16_t lightness;    /*!< Value of light lightness default state */
    u16_t hue;          /*!< Value of light hue default state */
    u16_t saturation;   /*!< Value of light saturation default state */
} ble_mesh_light_hsl_default_status_cb_t;

/** Parameters of Light HSL Range Status */
typedef struct {
    u8_t  status_code;          /*!< Status code for the request message */
    u16_t hue_range_min;        /*!< Value of hue range min field of light hsl hue range state */
    u16_t hue_range_max;        /*!< Value of hue range max field of light hsl hue range state */
    u16_t saturation_range_min; /*!< Value of saturation range min field of light hsl saturation range state */
    u16_t saturation_range_max; /*!< Value of saturation range max field of light hsl saturation range state */
} ble_mesh_light_hsl_range_status_cb_t;

/** Parameters of Light xyL Status */
typedef struct {
    bool  op_en;                /*!< Indicate whether optional parameters included */
    u16_t xyl_lightness;        /*!< The present value of the Light xyL Lightness state */
    u16_t xyl_x;                /*!< The present value of the Light xyL x state */
    u16_t xyl_y;                /*!< The present value of the Light xyL y state */
    u8_t  remain_time;          /*!< Time to complete state transition (optional) */
} ble_mesh_light_xyl_status_cb_t;

/** Parameters of Light xyL Target Status */
typedef struct {
    bool  op_en;                /*!< Indicate whether optional parameters included */
    u16_t target_xyl_lightness; /*!< The target value of the Light xyL Lightness state */
    u16_t target_xyl_x;         /*!< The target value of the Light xyL x state */
    u16_t target_xyl_y;         /*!< The target value of the Light xyL y state */
    u8_t  remain_time;          /*!< Time to complete state transition (optional) */
} ble_mesh_light_xyl_target_status_cb_t;

/** Parameters of Light xyL Default Status */
typedef struct {
    u16_t lightness;        /*!< The value of the Light Lightness Default state */
    u16_t xyl_x;            /*!< The value of the Light xyL x Default state */
    u16_t xyl_y;            /*!< The value of the Light xyL y Default state */
} ble_mesh_light_xyl_default_status_cb_t;

/** Parameters of Light xyL Range Status */
typedef struct {
    u8_t  status_code;      /*!< Status Code for the requesting message */
    u16_t xyl_x_range_min;  /*!< The value of the xyL x Range Min field of the Light xyL x Range state */
    u16_t xyl_x_range_max;  /*!< The value of the xyL x Range Max field of the Light xyL x Range state */
    u16_t xyl_y_range_min;  /*!< The value of the xyL y Range Min field of the Light xyL y Range state */
    u16_t xyl_y_range_max;  /*!< The value of the xyL y Range Max field of the Light xyL y Range state */
} ble_mesh_light_xyl_range_status_cb_t;

/** Parameter of Light LC Mode Status */
typedef struct {
    u8_t mode;              /*!< The present value of the Light LC Mode state */
} ble_mesh_light_lc_mode_status_cb_t;

/** Parameter of Light LC OM Status */
typedef struct {
    u8_t mode;              /*!< The present value of the Light LC Occupancy Mode state */
} ble_mesh_light_lc_om_status_cb_t;

/** Parameters of Light LC Light OnOff Status */
typedef struct {
    bool op_en;                 /*!< Indicate whether optional parameters included */
    u8_t present_light_onoff;   /*!< The present value of the Light LC Light OnOff state */
    u8_t target_light_onoff;    /*!< The target value of the Light LC Light OnOff state (Optional) */
    u8_t remain_time;           /*!< Time to complete state transition (C.1) */
} ble_mesh_light_lc_light_onoff_status_cb_t;

/** Parameters of Light LC Property Status */
typedef struct {
    u16_t property_id;      /*!< Property ID identifying a Light LC Property */
    struct net_buf_simple *property_value;  /*!< Raw value for the Light LC Property */
} ble_mesh_light_lc_property_status_cb_t;

/**
 * @brief Lighting Client Model received message union
 */
typedef union {
    ble_mesh_light_lightness_status_cb_t             lightness_status;             /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_STATUS */
    ble_mesh_light_lightness_linear_status_cb_t      lightness_linear_status;      /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LINEAR_STATUS */
    ble_mesh_light_lightness_last_status_cb_t        lightness_last_status;        /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_LAST_STATUS */
    ble_mesh_light_lightness_default_status_cb_t     lightness_default_status;     /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_DEFAULT_STATUS */
    ble_mesh_light_lightness_range_status_cb_t       lightness_range_status;       /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LIGHTNESS_RANGE_STATUS */
    ble_mesh_light_ctl_status_cb_t                   ctl_status;                   /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_STATUS */
    ble_mesh_light_ctl_temperature_status_cb_t       ctl_temperature_status;       /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_STATUS */
    ble_mesh_light_ctl_temperature_range_status_cb_t ctl_temperature_range_status; /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_TEMPERATURE_RANGE_STATUS */
    ble_mesh_light_ctl_default_status_cb_t           ctl_default_status;           /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_CTL_DEFAULT_STATUS */
    ble_mesh_light_hsl_status_cb_t                   hsl_status;                   /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_STATUS */
    ble_mesh_light_hsl_target_status_cb_t            hsl_target_status;            /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_TARGET_STATUS */
    ble_mesh_light_hsl_hue_status_cb_t               hsl_hue_status;               /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_HUE_STATUS */
    ble_mesh_light_hsl_saturation_status_cb_t        hsl_saturation_status;        /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_SATURATION_STATUS */
    ble_mesh_light_hsl_default_status_cb_t           hsl_default_status;           /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_DEFAULT_STATUS */
    ble_mesh_light_hsl_range_status_cb_t             hsl_range_status;             /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_HSL_RANGE_STATUS */
    ble_mesh_light_xyl_status_cb_t                   xyl_status;                   /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_STATUS */
    ble_mesh_light_xyl_target_status_cb_t            xyl_target_status;            /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_TARGET_STATUS */
    ble_mesh_light_xyl_default_status_cb_t           xyl_default_status;           /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_DEFAULT_STATUS */
    ble_mesh_light_xyl_range_status_cb_t             xyl_range_status;             /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_XYL_RANGE_STATUS */
    ble_mesh_light_lc_mode_status_cb_t               lc_mode_status;               /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_MODE_STATUS */
    ble_mesh_light_lc_om_status_cb_t                 lc_om_status;                 /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_OM_STATUS */
    ble_mesh_light_lc_light_onoff_status_cb_t        lc_light_onoff_status;        /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_LIGHT_ONOFF_STATUS */
    ble_mesh_light_lc_property_status_cb_t           lc_property_status;           /*!< For BLE_MESH_BLE_MESH_MODEL_OP_LIGHT_LC_PROPERTY_STATUS */
} ble_mesh_light_client_status_cb_t;

/** Lighting Client Model callback parameters */
typedef struct {
    int error_code;                                     /*!< Appropriate error code */
    ble_mesh_client_common_param_t   *params;       /*!< The client common parameters. */
    ble_mesh_light_client_status_cb_t status_cb;    /*!< The light status message callback values */
} ble_mesh_light_client_cb_param_t;

/** This enum value is the event of Lighting Client Model */
typedef enum {
    BLE_MESH_BLE_MESH_LIGHT_CLIENT_GET_STATE_EVT,
    BLE_MESH_BLE_MESH_LIGHT_CLIENT_SET_STATE_EVT,
    BLE_MESH_BLE_MESH_LIGHT_CLIENT_PUBLISH_EVT,
    BLE_MESH_BLE_MESH_LIGHT_CLIENT_TIMEOUT_EVT,
    BLE_MESH_BLE_MESH_LIGHT_CLIENT_EVT_MAX,
} ble_mesh_light_client_cb_event_t;

/**
 *  @brief Bluetooth Mesh Light Client Model function.
 */

/**
 * @brief   Lighting Client Model callback function type
 * @param   event: Event type
 * @param   param: Pointer to callback parameter
 */
typedef void (* ble_mesh_light_client_cb_t)(ble_mesh_light_client_cb_event_t event,
        ble_mesh_light_client_cb_param_t *param);

/**
 * @brief       Register BLE Mesh Light Client Model callback.
 *
 * @param[in]   callback: pointer to the callback function.
 *
 * @return      BT_OK on success or error code otherwise.
 *
 */
bt_err_t ble_mesh_register_light_client_callback(ble_mesh_light_client_cb_t callback);

/**
 * @brief       Get the value of Light Server Model states using the Light Client Model get messages.
 *
 * @note        If you want to know the opcodes and corresponding meanings accepted by this API,
 *              please refer to ble_mesh_light_message_opcode_t in yoc_ble_mesh_defs.h
 *
 * @param[in]   params:    Pointer to BLE Mesh common client parameters.
 * @param[in]   get_state: Pointer of light get message value.
 *                         Shall not be set to NULL.
 *
 * @return      BT_OK on success or error code otherwise.
 *
 */
bt_err_t ble_mesh_light_client_get_state(ble_mesh_client_common_param_t *params,
        ble_mesh_light_client_get_state_t *get_state);

/**
 * @brief       Set the value of Light Server Model states using the Light Client Model set messages.
 *
 * @note        If you want to know the opcodes and corresponding meanings accepted by this API,
 *              please refer to ble_mesh_light_message_opcode_t in yoc_ble_mesh_defs.h
 *
 * @param[in]   params:    Pointer to BLE Mesh common client parameters.
 * @param[in]   set_state: Pointer of generic set message value.
 *                         Shall not be set to NULL.
 *
 * @return      BT_OK on success or error code otherwise.
 *
 */
bt_err_t ble_mesh_light_client_set_state(ble_mesh_client_common_param_t *params,
        ble_mesh_light_client_set_state_t *set_state);


#endif /* _BLE_MESH_BLE_MESH_LIGHTING_MODEL_API_H_ */

