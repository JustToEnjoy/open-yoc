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
#include <errno.h>

#include "btc/btc_task.h"
#include "btc/btc_manage.h"
#include "osi/allocator.h"

#include "bt_err.h"
#include "yoc_bt_defs.h"
#include "yoc_bt_main.h"

#include "mesh.h"
#include "provisioner_main.h"
#include "btc_ble_mesh_prov.h"
#include "yoc_ble_mesh_networking_api.h"

#define BLE_MESH_TX_SDU_MAX ((CONFIG_BLE_MESH_ADV_BUF_COUNT - 3) * 12)

static bt_err_t ble_mesh_send_msg(ble_mesh_model_t *model,
                                   ble_mesh_msg_ctx_t *ctx,
                                   uint32_t opcode,
                                   btc_ble_mesh_model_act_t act,
                                   uint16_t length, uint8_t *data,
                                   int32_t msg_timeout, bool need_rsp,
                                   ble_mesh_dev_role_t device_role)
{
    btc_ble_mesh_model_args_t arg = {0};
    uint8_t op_len = 0, mic_len = 0;
    uint8_t *msg_data = NULL;
    btc_msg_t msg = {0};
    bt_err_t status;

    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    if (device_role > ROLE_FAST_PROV) {
        return BT_ERR_INVALID_ARG;
    }

    /* When data is NULL, it is mandatory to set length to 0 to prevent users from misinterpreting parameters. */
    if (data == NULL) {
        length = 0;
    }

    if (opcode < 0x100) {
        op_len = 1;
    } else if (opcode < 0x10000) {
        op_len = 2;
    } else {
        op_len = 3;
    }

    if (act == BTC_BLE_MESH_ACT_MODEL_PUBLISH) {
        if (op_len + length > model->pub->msg->size) {
            BT_LOG_ERROR("%s, Model publication msg size %d is too small", __func__, model->pub->msg->size);
            return BT_ERR_INVALID_ARG;
        }
    }

    if (act == BTC_BLE_MESH_ACT_MODEL_PUBLISH) {
        mic_len = 4;
    } else {
        mic_len = ctx->send_rel ? 8 : 4;
    }

    if (op_len + length + mic_len > MIN(BLE_MESH_BLE_MESH_SDU_MAX_LEN, BLE_MESH_TX_SDU_MAX)) {
        BT_LOG_ERROR("%s, Data length %d is too large", __func__, length);
        return BT_ERR_INVALID_ARG;
    }

    if (act == BTC_BLE_MESH_ACT_MODEL_PUBLISH) {
        bt_mesh_model_msg_init(model->pub->msg, opcode);
        net_buf_simple_add_mem(model->pub->msg, data, length);
    } else {
        msg_data = (uint8_t *)osi_malloc(op_len + length);
        if (msg_data == NULL) {
            return BT_ERR_NO_MEM;
        }
        ble_mesh_model_msg_opcode_init(msg_data, opcode);
        memcpy(msg_data + op_len, data, length);
    }

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_MODEL;
    msg.act = act;

    if (act == BTC_BLE_MESH_ACT_MODEL_PUBLISH) {
        arg.model_publish.model = model;
        arg.model_publish.device_role = device_role;
    } else {
        arg.model_send.model = model;
        arg.model_send.ctx = ctx;
        arg.model_send.need_rsp = need_rsp;
        arg.model_send.opcode = opcode;
        arg.model_send.length = op_len + length;
        arg.model_send.data = msg_data;
        arg.model_send.device_role = device_role;
        arg.model_send.msg_timeout = msg_timeout;
    }

    status = (btc_transfer_context(&msg, &arg, sizeof(btc_ble_mesh_model_args_t), btc_ble_mesh_prov_arg_deep_copy)
              == BT_STATUS_SUCCESS ? BT_OK : BT_FAIL);

    osi_free(msg_data);

    return status;
}

bt_err_t ble_mesh_register_custom_model_callback(ble_mesh_model_cb_t callback)
{
    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    return (btc_profile_cb_set(BTC_PID_MODEL, callback) == 0 ? BT_OK : BT_FAIL);
}

bt_err_t ble_mesh_model_msg_opcode_init(uint8_t *data, uint32_t opcode)
{
    uint16_t val;

    if (data == NULL) {
        return BT_ERR_INVALID_ARG;
    }

    if (opcode < 0x100) {
        /* 1-byte OpCode */
        data[0] = opcode & 0xff;
        return BT_OK;
    }

    if (opcode < 0x10000) {
        /* 2-byte OpCode, big endian */
        val = sys_cpu_to_be16 (opcode);
        memcpy(data, &val, 2);
        return BT_OK;
    }

    /* 3-byte OpCode, note that little endian for the least 2 bytes(Company ID) of opcode */
    data[0] = (opcode >> 16) & 0xff;
    val = sys_cpu_to_le16(opcode & 0xffff);
    memcpy(&data[1], &val, 2);

    return BT_OK;
}

bt_err_t ble_mesh_client_model_init(ble_mesh_model_t *model)
{
    if (model == NULL) {
        return BT_ERR_INVALID_ARG;
    }
    return btc_ble_mesh_client_init(model);
}

bt_err_t ble_mesh_server_model_send_msg(ble_mesh_model_t *model,
        ble_mesh_msg_ctx_t *ctx, uint32_t opcode,
        uint16_t length, uint8_t *data)
{
    if (!model || !ctx) {
        return BT_ERR_INVALID_ARG;
    }
    return ble_mesh_send_msg(model, ctx, opcode, BTC_BLE_MESH_ACT_SERVER_MODEL_SEND,
                             length, data, 0, false, ROLE_NODE);
}

bt_err_t ble_mesh_client_model_send_msg(ble_mesh_model_t *model,
        ble_mesh_msg_ctx_t *ctx, uint32_t opcode,
        uint16_t length, uint8_t *data, int32_t msg_timeout,
        bool need_rsp, ble_mesh_dev_role_t device_role)
{
    if (!model || !ctx) {
        return BT_ERR_INVALID_ARG;
    }
    return ble_mesh_send_msg(model, ctx, opcode, BTC_BLE_MESH_ACT_CLIENT_MODEL_SEND,
                             length, data, msg_timeout, need_rsp, device_role);
}

bt_err_t ble_mesh_model_publish(ble_mesh_model_t *model, uint32_t opcode,
                                     uint16_t length, uint8_t *data,
                                     ble_mesh_dev_role_t device_role)
{
    if (!model || !model->pub || !model->pub->msg) {
        return BT_ERR_INVALID_ARG;
    }
    return ble_mesh_send_msg(model, NULL, opcode, BTC_BLE_MESH_ACT_MODEL_PUBLISH,
                             length, data, 0, false, device_role);
}

bt_err_t ble_mesh_node_local_reset(void)
{
    btc_msg_t msg = {0};

    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_PROV;
    msg.act = BTC_BLE_MESH_ACT_NODE_RESET;

    return (btc_transfer_context(&msg, NULL, 0, NULL) == BT_STATUS_SUCCESS ? BT_OK : BT_FAIL);
}

#if (CONFIG_BLE_MESH_PROVISIONER)

bt_err_t ble_mesh_provisioner_set_node_name(int index, const char *name)
{
    btc_ble_mesh_prov_args_t arg = {0};
    btc_msg_t msg = {0};

    if (!name || (strlen(name) > BLE_MESH_BLE_MESH_NODE_NAME_MAX_LEN)) {
        return BT_ERR_INVALID_ARG;
    }

    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_PROV;
    msg.act = BTC_BLE_MESH_ACT_PROVISIONER_SET_NODE_NAME;

    arg.set_node_name.index = index;
    memset(arg.set_node_name.name, 0, sizeof(arg.set_node_name.name));
    memcpy(arg.set_node_name.name, name, strlen(name));
    return (btc_transfer_context(&msg, &arg, sizeof(btc_ble_mesh_prov_args_t), NULL)
            == BT_STATUS_SUCCESS ? BT_OK : BT_FAIL);
}

const char *ble_mesh_provisioner_get_node_name(int index)
{
    return bt_mesh_provisioner_get_node_name(index);
}

int ble_mesh_provisioner_get_node_index(const char *name)
{
    if (!name || (strlen(name) > BLE_MESH_BLE_MESH_NODE_NAME_MAX_LEN)) {
        return -EINVAL;
    }

    return bt_mesh_provisioner_get_node_index(name);
}

bt_err_t ble_mesh_provisioner_add_local_app_key(const uint8_t app_key[16],
        uint16_t net_idx, uint16_t app_idx)
{
    btc_ble_mesh_prov_args_t arg = {0};
    btc_msg_t msg = {0};

    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_PROV;
    msg.act = BTC_BLE_MESH_ACT_PROVISIONER_SET_LOCAL_APP_KEY;

    arg.add_local_app_key.net_idx = net_idx;
    arg.add_local_app_key.app_idx = app_idx;
    if (app_key) {
        memcpy(arg.add_local_app_key.app_key, app_key, 16);
    } else {
        bzero(arg.add_local_app_key.app_key, 16);
    }
    return (btc_transfer_context(&msg, &arg, sizeof(btc_ble_mesh_prov_args_t), NULL)
            == BT_STATUS_SUCCESS ? BT_OK : BT_FAIL);
}

const uint8_t *ble_mesh_provisioner_get_local_app_key(uint16_t net_idx, uint16_t app_idx)
{
    return bt_mesh_provisioner_local_app_key_get(net_idx, app_idx);
}

bt_err_t ble_mesh_provisioner_bind_app_key_to_local_model(uint16_t element_addr, uint16_t app_idx,
        uint16_t model_id, uint16_t company_id)
{
    btc_ble_mesh_prov_args_t arg = {0};
    btc_msg_t msg = {0};

    if (!BLE_MESH_ADDR_IS_UNICAST(element_addr)) {
        return BT_ERR_INVALID_ARG;
    }

    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_PROV;
    msg.act = BTC_BLE_MESH_ACT_PROVISIONER_BIND_LOCAL_MOD_APP;

    arg.local_mod_app_bind.elem_addr = element_addr;
    arg.local_mod_app_bind.app_idx = app_idx;
    arg.local_mod_app_bind.model_id = model_id;
    arg.local_mod_app_bind.cid = company_id;
    return (btc_transfer_context(&msg, &arg, sizeof(btc_ble_mesh_prov_args_t), NULL)
            == BT_STATUS_SUCCESS ? BT_OK : BT_FAIL);
}

bt_err_t ble_mesh_provisioner_add_local_net_key(const uint8_t net_key[16], uint16_t net_idx)
{
    btc_ble_mesh_prov_args_t arg = {0};
    btc_msg_t msg = {0};

    if (net_idx == BLE_MESH_KEY_PRIMARY) {
        return BT_ERR_INVALID_ARG;
    }

    BT_BLUEDROID_STATUS_CHECK(BT_BLUEDROID_STATUS_ENABLED);

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_PROV;
    msg.act = BTC_BLE_MESH_ACT_PROVISIONER_ADD_LOCAL_NET_KEY;

    arg.add_local_net_key.net_idx = net_idx;
    if (net_key) {
        memcpy(arg.add_local_net_key.net_key, net_key, 16);
    } else {
        bzero(arg.add_local_net_key.net_key, 16);
    }
    return (btc_transfer_context(&msg, &arg, sizeof(btc_ble_mesh_prov_args_t), NULL)
            == BT_STATUS_SUCCESS ? BT_OK : BT_FAIL);
}

const uint8_t *ble_mesh_provisioner_get_local_net_key(uint16_t net_idx)
{
    return bt_mesh_provisioner_local_net_key_get(net_idx);
}

#endif /* CONFIG_BLE_MESH_PROVISIONER */

#if (CONFIG_BLE_MESH_FAST_PROV)
const uint8_t *ble_mesh_get_fast_prov_app_key(uint16_t net_idx, uint16_t app_idx)
{
    return bt_mesh_get_fast_prov_app_key(net_idx, app_idx);
}
#endif  /* CONFIG_BLE_MESH_FAST_PROV */

