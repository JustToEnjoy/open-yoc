/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */
#include <app_config.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <aos/aos.h>
#include "aos/cli.h"
#include <uservice/uservice.h>
#include <uservice/eventid.h>
#include <media.h>
#include <yoc/mic.h>
#include <player.h>

#include "app_init.h"
#include "app_main.h"
#include "audio/audio_res.h"

#define TAG "app"

static void media_evt(int type, aui_player_evtid_t evt_id)
{
    //LOGD(TAG, "media_evt type %d,evt_id %d", type, evt_id);

    if (type == MEDIA_MUSIC) {
        switch (evt_id) {
            case AUI_PLAYER_EVENT_START:
                LOGD(TAG, "audio player start %d", AUI_PLAYER_EVENT_START);
                break;
            case AUI_PLAYER_EVENT_ERROR:
                local_audio_play(LOCAL_AUDIO_PLAY_ERR);
                LOGD(TAG, "audio player error %d", AUI_PLAYER_EVENT_ERROR);
                lpm_update();
                break;
            case AUI_PLAYER_EVENT_FINISH:
                LOGD(TAG, "audio player finish %d", AUI_PLAYER_EVENT_FINISH);
                lpm_update();
                break;
            default:
                break;
        }
        return;
    }
}

static int app_media_init(utask_t *task)
{
    int ret = -1;

    ret = aui_player_init(task, media_evt);

    return ret;
}

void mic_evt_cb(int source, mic_event_id_t evt_id, void *data, int size)
{
    int        ret;
    static int pcm_started;

    switch (evt_id) {
        case MIC_EVENT_MIC_DATA:
            app_rec_copy_data(0, data, size);
            break;
        case MIC_EVENT_REF_DATA:
            app_rec_copy_data(1, data, size);
            break;
        case MIC_EVENT_PCM_DATA:
            app_rec_copy_data(2, data, size);
            if (pcm_started == 0)
                break;
            /* 麦克风数据，推到云端 */
            ret = app_aui_cloud_push_audio(data, size);
            if (ret < 0) {
                /* 数据推送错误 */
                pcm_started = 0;
                LOGE(TAG, "cloud push pcm finish");
                aui_mic_control(MIC_CTRL_STOP_PCM);
                // ret = aui_cloud_stop_pcm(&g_aui_handler);
                app_aui_cloud_stop(1);
                if (wifi_internet_is_connected() == 0) {
                    LOGE(TAG, "00mic evt ntp not synced");
                    local_audio_play(LOCAL_AUDIO_NET_FAIL);
                } else {
                    if (ret < 0) {
                        local_audio_play(LOCAL_AUDIO_SORRY2);
                    }
                }
            }
            break;

        case MIC_EVENT_VAD: {
            int enable = -1;
            ret = aos_kv_getint("vad_en", &enable);
            if ((ret == 0) && (enable == 1)) {
                // LOGD(TAG, "MIC_EVENT_VAD");
                lpm_update();
            }
            break;
        }

        case MIC_EVENT_SESSION_START:
            LOGE(TAG, "asr ok");

            lpm_update();
            if (app_player_get_mute_state()) {
                return;
            }
            
            /* 网络检测 */
            if (wifi_internet_is_connected() == 0) {
                LOGE(TAG, "mic_evt net connect failed");
                aui_mic_control(MIC_CTRL_STOP_PCM);
                app_aui_cloud_stop(1);
                if (wifi_is_pairing())
                    local_audio_play(LOCAL_AUDIO_NET_CFG_CONFIG);
                else
                    local_audio_play(LOCAL_AUDIO_NET_FAIL);
                return;
            }

            app_aui_cloud_start(0);
            /* 开始交互 */
            if (pcm_started == 0) {
                pcm_started = 1;
                local_audio_play(LOCAL_AUDIO_HELLO);
                aos_msleep(500);
                // app_status_update();
                aui_mic_control(MIC_CTRL_START_PCM);
            }
            break;
        case MIC_EVENT_SESSION_STOP:
            /* 交互结束 */
            lpm_update();
            if (pcm_started == 1) {
                pcm_started = 0;
                LOGI(TAG, "asr session finish");
                aui_mic_control(MIC_CTRL_STOP_PCM);
                app_aui_cloud_stop(0);
            }

            break;
        default:;
    }
}

int app_mic_is_busy(void)
{
    mic_state_t mic_st;
    if (aui_mic_get_state(&mic_st) == 0) {
        if (mic_st == MIC_STATE_SESSION) {
            return 1;
        }
    }

    return 0;
}

/* mic init */
static int app_mic_init(utask_t *task)
{
    int ret;
    static voice_adpator_param_t voice_param;

    voice_mic_register();

    ret = aui_mic_init(task, mic_evt_cb);

    if (ret < 0) {
        return ret;
    }

    mic_param_t p;

    memset(&p, 0x00, sizeof(mic_param_t));

    voice_param.pcm         = "pcmC0";
    voice_param.cts_ms      = 80;
    voice_param.ipc_mode    = 1;

    p.channels          = 1;
    p.sample_bits       = 16;
    p.rate              = 16000;
    p.priv              = &voice_param;

    aui_mic_set_param(&p);

    ret = aui_mic_start();

    return ret;
}

/*************************************************
 * APP的各种测试命令
 *************************************************/
static void cli_reg_cmd_app(void)
{
    extern void cli_reg_cmd_aui(void);
    cli_reg_cmd_aui();

    extern void cli_reg_cmd_apps(void);
    cli_reg_cmd_apps();

    //extern void test_gpio_register_cmd(void);
    //test_gpio_register_cmd();

    // extern void cli_yv_test_init(void);
    // cli_yv_test_init();

    //extern void test_register_system_cmd(void);
    //test_register_system_cmd();

    extern void cli_reg_cmd_iperf(void);
    cli_reg_cmd_iperf();
}

int main(void)
{
extern int posix_init(void);
extern void cxx_system_init(void);

    // must call this func when use c++
    posix_init();
    // must call this func when use c++
    cxx_system_init();

    board_yoc_init();

    app_lpm_init();

    wifi_mode_e mode = app_network_init();

    if (mode != MODE_WIFI_TEST) {
        /* 启动播放器 */
        utask_t *task_player = utask_new("app_player", 2 * 1024, QUEUE_MSG_COUNT, AOS_DEFAULT_APP_PRI);
        app_media_init(task_player);

        /* 启动麦克风服务 */
        utask_t *task_mic = utask_new("task_mic", 2 * 1024, QUEUE_MSG_COUNT, AOS_DEFAULT_APP_PRI);
        app_mic_init(task_mic);

        app_player_init();

        if (mode != MODE_WIFI_PAIRING)
            local_audio_play(LOCAL_AUDIO_STARTING);

        app_fota_init();

        app_button_init();
    }

    /* 交互系统初始化 */
    app_aui_init();

    /* 命令行测试命令 */
    cli_reg_cmd_app();

#ifdef CONFIG_LED_TASK_ON
    /* LED task */
    led_test_task();
#endif
	return 0;
}

