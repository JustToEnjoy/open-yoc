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

#include <stdint.h>

#include "btc/btc_task.h"
#include "btc/btc_manage.h"

#include "yoc_bt_defs.h"
#include "yoc_bt_main.h"

#include "btc_ble_mesh_health_model.h"
#include "yoc_ble_mesh_health_model_api.h"

bt_err_t ble_mesh_register_health_client_callback(ble_mesh_health_client_cb_t callback)
{
    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    return (btc_profile_cb_set(BTC_PID_HEALTH_CLIENT, callback) == 0 ? BT_OK : BT_FAIL);
}

bt_err_t ble_mesh_register_health_server_callback(ble_mesh_health_server_cb_t callback)
{
    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    return (btc_profile_cb_set(BTC_PID_HEALTH_SERVER, callback) == 0 ? BT_OK : BT_FAIL);
}

bt_err_t ble_mesh_health_client_get_state(ble_mesh_client_common_param_t *params,
        ble_mesh_health_client_get_state_t *get_state)
{
    btc_ble_mesh_health_client_args_t arg = {0};
    btc_msg_t msg = {0};

    if (!params || !params->model || !params->ctx.addr || !get_state) {
        return BT_ERR_INVALID_ARG;
    }

    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_HEALTH_CLIENT;
    msg.act = BTC_BLE_MESH_ACT_HEALTH_CLIENT_GET_STATE;
    arg.health_client_get_state.params = params;
    arg.health_client_get_state.get_state = get_state;

    return (btc_transfer_context(&msg, &arg, sizeof(btc_ble_mesh_health_client_args_t), btc_ble_mesh_health_client_arg_deep_copy)
            == BT_STATUS_SUCCESS ? BT_OK : BT_FAIL);
}

bt_err_t ble_mesh_health_client_set_state(ble_mesh_client_common_param_t *params,
        ble_mesh_health_client_set_state_t *set_state)
{
    btc_ble_mesh_health_client_args_t arg = {0};
    btc_msg_t msg = {0};

    if (!params || !params->model || !params->ctx.addr || !set_state) {
        return BT_ERR_INVALID_ARG;
    }

    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_HEALTH_CLIENT;
    msg.act = BTC_BLE_MESH_ACT_HEALTH_CLIENT_SET_STATE;
    arg.health_client_set_state.params = params;
    arg.health_client_set_state.set_state = set_state;

    return (btc_transfer_context(&msg, &arg, sizeof(btc_ble_mesh_health_client_args_t), btc_ble_mesh_health_client_arg_deep_copy)
            == BT_STATUS_SUCCESS ? BT_OK : BT_FAIL);
}

bt_err_t ble_mesh_health_server_fault_update(ble_mesh_elem_t *element)
{
    btc_ble_mesh_health_server_args_t arg = {0};
    btc_msg_t msg = {0};

    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_HEALTH_SERVER;
    msg.act = BTC_BLE_MESH_ACT_HEALTH_SERVER_FAULT_UPDATE;
    arg.fault_update.element = element;

    return (btc_transfer_context(&msg, &arg, sizeof(btc_ble_mesh_health_server_args_t), NULL)
            == BT_STATUS_SUCCESS ? BT_OK : BT_FAIL);
}
