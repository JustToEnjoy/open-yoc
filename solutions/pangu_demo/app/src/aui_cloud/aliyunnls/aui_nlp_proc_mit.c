/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */

#include "app_main.h"
#include "aui_nlp.h"
#include "app_config.h"

#define TAG "nlpmit"

/*
一次交互会有asr和nlp两次事件，但两次事件的task id相同，
如果asr处理是对该值赋值，说明要忽略后续的nlp处理。
*/
static char g_nlp_task_id[40] = {0};

/**
 * 解析ASR&NLP信息
*/
int aui_nlp_proc_mit(cJSON *js, const char *json_text)
{
    int ret = -1;

    LOGD(TAG, "Enter %s \n<<<<\n%s\n>>>>", __FUNCTION__, json_text);

    cJSON *name = cJSON_GetObjectItemByPath(js, "header.name");

    if (!cJSON_IsString(name)) {
        return AUI_CMD_PROC_ERROR;
    }

    if (strcmp(name->valuestring, "RecognitionCompleted") == 0) {
        /* ASR 结果 */
        cJSON *asr_result = cJSON_GetObjectItemByPath(js, "payload.result");
        if (cJSON_IsString(asr_result)) {
            LOGD(TAG, "ASR Result:\n<<<<%s>>>>\n", asr_result->valuestring);
            ret = aui_nlp_proc_textcmd(asr_result->valuestring);
            if (ret == 0) {
                cJSON *task_id = cJSON_GetObjectItemByPath(js, "header.task_id");
                if (cJSON_IsString(task_id)) {
                    /* 忽略后面的NLP处理 */
                    strncpy(g_nlp_task_id, task_id->valuestring, sizeof(g_nlp_task_id) - 1);
                    g_nlp_task_id[sizeof(g_nlp_task_id) - 1] = 0;
                }
            }
            return 0;
        }

        return AUI_CMD_PROC_MATCH_NOACTION;
    } else if (strcmp(name->valuestring, "DialogResultGenerated") == 0) {
        int done = 0;

        cJSON *task_id = cJSON_GetObjectItemByPath(js, "header.task_id");
        if (json_string_eq(task_id, g_nlp_task_id)) {
            g_nlp_task_id[0] = '\0';
            return 0;
        }

        /* NLP 结果，TTS前执行动作 */
        cJSON *action = cJSON_GetObjectItemByPath(js, "payload.action");
        if (cJSON_IsString(action)) {
            LOGD(TAG, "NLP Action:\n<<<<%s>>>>\n", action->valuestring);
            if (strcmp(action->valuestring, "play_music") == 0) {
                /* may be not contains "action_params" in json */
                ret = aui_nlp_action_play(js);
                if (ret == 0) {
                    done = 1;
                    goto mit_nlp_end;
                }
            } else if (strcmp(action->valuestring, "Action://audio/pause") == 0) {
                local_audio_play(LOCAL_AUDIO_OK);
                aui_player_pause(MEDIA_MUSIC);
                done = 1;
            } else if (strcmp(action->valuestring, "Action://audio/resume") == 0) {
                aui_player_resume(MEDIA_MUSIC);
                done = 1;
            } else if (strcmp(action->valuestring, "Action://audio/next") == 0) {
                LOGI(TAG, "Do %s\n", action->valuestring);
                done = 1;
            } else if (strcmp(action->valuestring, "Action://audio/previous") == 0) {
                LOGI(TAG, "Do %s\n", action->valuestring);
                done = 1;
            } else if (strcmp(action->valuestring, "Action://alarm/add_alarm") == 0
                       || strcmp(action->valuestring, "Action://alarm/set_alarm") == 0) {
                // aui_nlp_action_set_alarm(js, NLP_ACTION_ALARM_ADD);
                done = 1;
            } else if (strcmp(action->valuestring, "Action://alarm/delete_alarm") == 0) {
                // aui_nlp_action_set_alarm(js, NLP_ACTION_ALARM_DEL);
                done = 1;
#if defined(APP_PWM_EN) && APP_PWM_EN
            } else if (strcmp(action->valuestring, "Action://house_control/start_device") == 0) {
                app_pwm_led_control(1);
                done = 1;
            } else if (strcmp(action->valuestring, "Action://house_control/stop_device") == 0) {
                app_pwm_led_control(0);
                done = 1;
            } else if (strcmp(action->valuestring, "Action://house_control/set_light") == 0) {
                ret = aui_nlp_action_set_light(js);
                if (ret == 0) {
                    done = 1;
                }
#endif //APP_PWM_EN
            }
        }

        /* NLP 结果，语音交互 */

        cJSON *spoken_text = cJSON_GetObjectItemByPath(js, "payload.spoken_text");
        if (!done && cJSON_IsString(spoken_text)) {
            LOGD(TAG, "NLP Result:\n<<<<%s>>>>\n", spoken_text->valuestring);
            if (strlen(spoken_text->valuestring) > 0) {
                app_aui_cloud_tts_run(spoken_text->valuestring, 0);
                // app_aui_cloud_tts_wait_start();
            }
            done = 1;
        } else if(done) {
            /* 如果云端不带TTS，本地处理完毕，播放提示音‘好的’ */
            // LOGD(TAG, "好的！");
            // app_aui_cloud_tts_run("好的", 0);
            // app_aui_cloud_tts_wait_start();
        }


mit_nlp_end:
        return done ? 0 : AUI_CMD_PROC_MATCH_NOACTION;
    } else {
        return AUI_CMD_PROC_NOMATCH;
    }

    return 0;
}
