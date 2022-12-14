## 1.概述
`genie_mesh_switches` 是可连接天猫精灵音箱，同时支持天猫精灵APP控制的三键开关的参考应用，可以作为零火开关或者其他多Element Mesh设备的开发示例。目前只适配了TG7120B，且由于TG7120B芯片暂未适配低功耗，所以不推荐做单火开关应用。如果开发风扇灯、多孔插座等产品可以参考这个应用进行产品开发。
参考应用运行平台是TG7120B QFN32封装的开发板，这个开发板上只有两个独立按键，其中P14按键对应Element 0，P15按键对应Element 1，另外引脚P24对应Element 2，用一个跳线短接GND地线与P24引脚，即可模拟按键。

- 三键开关应用按键功能定义如下表所示：
| GPIO定义 | 短按 |
| --- | --- |
| P14 （Element 0） | 翻转Element 0 开关状态
翻转红灯开关 |
| P15 （Element 1） | 翻转Element 1 开关状态
翻转绿灯开关 |
| P24（Element 2） | 翻转Element 2 开关状态
翻转蓝灯开关 |

- 三键开关固件默认配置关闭了低功耗功能
​
## 2.编译

```bash
cd solutions/genie_mesh_switches/
make clean
make
```

生成固件：
完整的烧录固件：solutions/genie_mesh_switches/generated/total_image.hexf
OTA固件：solutions/genie_mesh_switches/generated/fota.bin

## 3.烧录

当前`genie_mesh_switches` 只适配了TG7120B。

### 3.1 烧录TG7120B

参考boards/TG7120B_evb/README.md中的烧录方法。

## 4.启动
烧录完成之后按复位键或者重新上电，启动设备
TG7120B 调试串口默认波特率为256000，连接串口工具后串口会有打印输出


## 5.说明
### 5.1.对Genie Mesh SDK的接口调用说明
#### 5.1.1 SDK初始化
函数名称：int genie_service_init(genie_service_ctx_t* p_ctx)
参数说明：
结构体genie_service_ctx_t的成员参数如下：

| 参数名称 | 参数类型 | 功能说明 | 备注 |
| --- | --- | --- | --- |
| p_mesh_elem | structbt_mesh_elem | Element的地址 |  |
| mesh_elem_counts | uint8_t | Element的数目 |  |
| event_cb | user_event_cb | SDK上传给应用的事件 | 用户事件回调，系统初始化和复位通知及接收Mesh数据通知，另外还有定时事件和用户自定义串口透传数据通知 |
| prov_timeout | uint32_t | 默认10分钟，超时之后进入静默广播状态 | 静默广播可以进行设备发现，但是不能直接配网 |
| lpm_conf | genie_lpm_conf_t | 低功耗相关参数配置 | 低功耗相关，由宏定义CONFIG_PM_SLEEP开关该功能
（芯片尚未适配，低功耗暂不推荐打开） |

结构体genie_lpm_conf_t的成员参数如下：

| 参数名称 | 参数类型 | 功能说明 | 备注 |
| --- | --- | --- | --- |
| lpm_wakeup_io | uint8_t | 是否支持GPIO唤醒，0：不支持，>0：支持 |  |
| lpm_wakeup_io_config | genie_lpm_wakeup_io_t | 配置多个唤醒GPIO | 多键开关可以使用 |
| is_auto_enable | uint8_t | 上电之后是否自动进入低功耗状态 | 大于0自动进入低功耗 |
| delay_sleep_time | uint32_t | 上电多久之后进入低功耗状态，默认是10秒 |  |
| sleep_ms | uint16_t | 低功耗睡眠时长 | GLP模式的时候1100ms |
| wakeup_ms | uint16_t | 低功耗唤醒时长 | GLP模式的时候60ms |
| genie_lpm_cb | genie_lpm_cb_t | 睡眠和唤醒状态通知用户的回调函数 |  |

#### 5.1.2 发送Mesh数据
SDK为Genie Vendor Model的数据发送提供了两个API接口，一个需要element参数，另外一个不需要element参数，会直接使用primary element，两个函数的参数介绍如下：
函数名称：int genie_transport_send_model(genie_transport_model_param_t* p_vendor_msg)
参数说明：
结构体genie_transport_model_param_t的成员参数如下：

| 参数名称 | 参数类型 | 功能说明 | 备注 |
| --- | --- | --- | --- |
| p_elem | struct bt_mesh_elem* | 指定发送数据的Element，主要是获取源地址 |  |
| p_model | struct bt_mesh_model* | 指定发送数据的model，主要是获取app key |  |
| result_cb | transport_result_cb | 发送结果回调给应用 | 发送超时或者发送成功（D3和CF消息不会执行回调） |
| retry | uint8_t | 没有收到应答执行重发的次数，默认重发两次 | 如果是GLP设备默认重发五次 |
| opid | uint8_t | opcode id，例如：0xD4 | 取3字节opcode的第一个字节 |
| tid | uint8_t | 传输ID，通常传入值0，为0的时候SDK会自动生成TID | Mesh设备发送数据的TID从0x80到0xBF |
| dst_addr | uint16_t | 数据目的地址 |  |
| retry_period | uint16_t | 重传的时间间隔 |  |
| len | uint16_t | 发送数据的长度 |  |
| data | uint8_t*  | 发送数据的起始地址 |  |

参考代码如下，可以调用bt_mesh_elem_find_by_id函数获取对应的element指针：
```c
genie_transport_model_param_t transport_model_param = {0};

memset(&transport_model_param, 0, sizeof(genie_transport_model_param_t));
transport_model_param.opid = VENDOR_OP_ATTR_INDICATE;
transport_model_param.data = payload;
transport_model_param.len = MIN(SIG_MODEL_INDICATE_PAYLOAD_MAX_LEN, payload_len);
transport_model_param.p_elem = bt_mesh_elem_find_by_id(p_elem->element_id);
transport_model_param.retry_period = GENIE_TRANSPORT_EACH_PDU_TIMEOUT * genie_transport_get_seg_count(transport_model_param.len);
transport_model_param.retry = GENIE_TRANSPORT_DEFAULT_RETRY_COUNT;

return genie_transport_send_model(&transport_model_param);
```
函数名称：int genie_transport_send_payload(genie_transport_payload_param_t* payload_param)
结构体genie_transport_payload_param_t的成员参数如下：

| 参数名称 | 参数类型 | 功能说明 | 备注 |
| --- | --- | --- | --- |
| opid | uint8_t | opcode id，例如：0xD4 | 取3字节opcode的第一个字节 |
| tid | uint8_t | 传输ID，通常传入值0，为0的时候SDK会自动生成TID | Mesh设备发送数据的TID从0x80到0xBF |
| retry_cnt | uint8_t | 没有收到应答执行重发的次数，默认重发两次 | 如果是GLP设备默认重发五次 |
| dst_addr | uint16_t | 数据目的地址 |  |
| p_payload | uint8_t*  | 发送数据的起始地址 |  |
| payload_len | uint16_t | 发送数据的长度 |  |
| result_cb | transport_result_cb | 发送结果回调给应用 | 发送超时或者发送成功（D3和CF消息不会执行回调） |

```c
genie_transport_payload_param_t transport_payload_param;

memset(&transport_payload_param, 0, sizeof(genie_transport_payload_param_t));
transport_payload_param.opid = VENDOR_OP_ATTR_INDICATE;
transport_payload_param.p_payload = payload;
transport_payload_param.payload_len = sizeof(payload);
transport_payload_param.retry_cnt = GENIE_TRANSPORT_DEFAULT_RETRY_COUNT;
transport_payload_param.result_cb = report_poweron_callback;

return genie_transport_send_payload(&transport_payload_param);
```
#### 5.1.3 接收Mesh数据
应用程序接收Mesh数据是通过初始化的时候注册的用户回调函数接收的，如下两个Event分别接收SIG Model的数据和Genie的Vendor Model的数据：

- 接收SIG Model的数据
```c
      case GENIE_EVT_SIG_MODEL_MSG:    {
        sig_model_msg *p_msg = (sig_model_msg *)p_arg;

        if (p_msg)
        {
            GENIE_LOG_INFO("SIG mesg ElemID(%d)", p_msg->element_id);
        }
    }
    break;
```


- 接收Vendor Model的数据
```c
    case GENIE_EVT_VENDOR_MODEL_MSG:
    {
        genie_transport_model_param_t *p_msg = (genie_transport_model_param_t *)p_arg;

        if (p_msg && p_msg->p_model && p_msg->p_model->user_data)
        {
            sig_model_element_state_t *p_elem_state = (sig_model_element_state_t *)p_msg->p_model->user_data;
            GENIE_LOG_INFO("ElemID(%d) TID(%02X)", p_elem_state->element_id, p_msg->tid);
            (void)p_elem_state;
        }
    }
    break;
```
### 5.2 多Element的定义
参考代码如下，多element开关，每个element通过宏定义GENIE_ADDR_SWITCH配置了开关的品类组播地址。
如果开发别的类型的设备，注意每个Element可以定义不同的组播地址。例如如果开发风扇灯的话，可以分别填灯的品类组播地址和一个风扇的品类组播地址。
```c
struct bt_mesh_elem switches_elements[] = {
    BT_MESH_ELEM(0, primary_element, primary_vendor_element, GENIE_ADDR_SWITCH),
    BT_MESH_ELEM(0, secondary_element, secondary_vendor_element, GENIE_ADDR_SWITCH),
    BT_MESH_ELEM(0, third_element, third_vendor_element, GENIE_ADDR_SWITCH),
};
```
 
### 5.3 主要串口命令说明
| 命令名称 | 命令说明 | 使用参考（示例） |
| --- | --- | --- |
| set_tt | 设备蓝牙Mesh设备三元组 | set_tt 5297793 0c51b11c6ec78b52b803b3bbaae64fba 486e704a5bf6 |
| get_tt | 查看蓝牙Mesh设备三元组 | 无参数 |
| get_info | 查看版本和MAC等信息 | 无参数 |
| reboot | 系统重启 | 无参数 |
| reset | 设备复位 | 无参数 |
| mesg | 通过MESH发送数据 | mesg d4 1 f000 010203 |

mesg命令参数说明：

1. 第一个参数d4就是indication发送，其他有D3、CE及CF等；
1. 第二个参数是发送模式和重发次数参数
- 0表示不重发
- 1-252表示重发次数
- 253表示使用payload的第一个字节作为时间间隔参数，单位是100ms，例如：mesg d4 253 f000 030201  表示300毫秒发一次0201，mesg d4 253 f000 1e0201是3秒一次0201
- 254表示收到回复或者发送超时就再次发送
- 255表示每秒自动发送一次
3. 第三个参数是接收者地址，必须是四个字符如果设置为0000会使用音箱默认组播地址F000；
3. 第四个参数是发送的内容，例如010203就是发送0x01，0x02，0x03因此必须是偶数个0-f之间的字符；

### 5.4 宏定义说明
#### 5.4.1 开关功能相关宏定义
| 宏定义的名称 | 功能说明 |
| --- | --- |
| CONFIG_MESH_MODEL_GEN_ONOFF_SRV | 支持开关控制功能 |


#### 5.4.2 其他通用功能相关宏定义
| 宏定义的名称 | 功能说明 |
| --- | --- |
| CONFIG_BT_MESH_GATT_PROXY | 支持Proxy功能 |
| CONFIG_BT_MESH_PB_GATT | 支持手机配网功能 |
| CONFIG_BT_MESH_RELAY | 支持中继功能 |
| CONFIG_GENIE_OTA | 支持手机OTA功能 |
| CONFIG_GENIE_RESET_BY_REPEAT | 支持连续上电五次进入配网状态功能 |
| PROJECT_SW_VERSION | 配置版本号，OTA功能使用，int32数据类型 |
| CONFIG_DEBUG | 支持BT_xxx日志输出 |
| CONFIG_BT_DEBUG_LOG | 支持BT_DBG日志输出 |
| MESH_DEBUG_PROV | 支持配网日志输出 |
| MESH_DEBUG_TX | 支持Access层发送Mesh数据日志输出 |
| MESH_DEBUG_RX | 支持Access层接收Mesh数据日志输出 |

- 本应用暂未适配TG7121B芯片

### 5.5 其他注意事项

- 按不同的按键设备上报每个开关的状态，不同的开关属于不同的Element，因此他们的地址不一样，注意从日志中观察发送数据的SRC地址。
- 给不同的开关配置的不同的位置、不同的名字，使用APP或者音箱单独操控某个开关，注意观察日志中目标地址应该对应每个开关的地址。
- 使用语言查询每个开关的状态，开关状态应该正确。
- 语言打开或者关闭全部开关应该支持。

## 6.其他参考文档
- [蓝牙mesh智能家居产品规范](https://help.aliyun.com/document_detail/173319.html?spm=a2c4g.11186623.6.694.77dc41f0KNwhT0)

- [蓝牙设备属性表](https://help.aliyun.com/document_detail/173320.html?spm=a2c4g.11186623.6.694.51385ed8bpGvnn)

- [蓝牙Mesh模组软件规范](https://help.aliyun.com/document_detail/173310.html?spm=a2c4g.11174283.6.689.2d197132x9Gqoi)

- [蓝牙Mesh设备扩展协议](https://help.aliyun.com/document_detail/173311.html?spm=a2c4g.11186623.6.685.6e5c59eev3fdLS)

- [蓝牙mesh设备开发FAQ](https://help.aliyun.com/document_detail/173313.html?spm=a2c4g.11186623.6.686.6fdb14262iF9B3)