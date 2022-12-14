#include "app_config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <aos/cli.h>
#include <yoc/mic.h>
#include <yoc/lpm.h>
#include "app_main.h"

#define TAG "app_cli"

static int cli_pair_proc(int argc, char **argv)
{
    if (argc < 3) {
        return -1;
    }
    if (strcmp(argv[2], "ap") == 0) {
        wifi_pair_start();
        return 0;
    }
    printf("app pair ap\n");
    return -1;
}

static int cli_mic_rec_proc(int argc, char **argv)
{
    if (argc < 3) {
        return -1;
    }
    if (strcmp(argv[1], "micrec") == 0) {
        if (strcmp(argv[2], "start") == 0) {
            if (argc > 4) {
                LOGD(TAG, "mic rec start.");
                // aui_mic_control(MIC_CTRL_STOP_PCM);
                app_rec_start(argv[3], argv[4]);
                return 0;
            }
        } else if (strcmp(argv[2], "stop") == 0) {
            LOGD(TAG, "mic rec stop");
            app_rec_stop();
            return 0;
        }
        printf("usage:\n");
        printf("app micrec stop\n");
        printf("app micrec start ws://192.168.1.102:8090 micdata.pcm\n");
    }

    return -1;
}

static void cmd_apps_func(char *wbuf, int wbuf_len, int argc, char **argv)
{
    if (argc >= 2) {
        if (strcmp(argv[1], "pair") == 0) {
            if (cli_pair_proc(argc, argv) == 0) {
                return;
            }            
        } else if(strcmp(argv[1], "micrec") == 0) {
            if (cli_mic_rec_proc(argc, argv) == 0) {
                return;
            }
        } else if(strcmp(argv[1], "lpm") == 0) {
            if (argc == 3) {
                int mode = atoi(argv[2]);

                if (mode == 1) {
                    pm_config_policy(LPM_POLICY_LOW_POWER);
                } else if (mode == 2) {
                    pm_config_policy(LPM_POLICY_DEEP_SLEEP);
                }
                if (lpm_check()) {
                    pm_agree_halt(0);
                } else {
                    printf("\tlpm check failed\n");
                }
            }
        }
    } else {
        printf("\tapp pair ap\n");
        printf("\tapp micrec ...\n");
    }
}


void test_register_system_cmd(void)
{
    extern void iwpriv(char *wbuf, int wbuf_len, int argc, char **argv);
    static const struct cli_command cmd_info = {
        "iwpriv",
        "test iwpriv",
        iwpriv
    };
    aos_cli_register_command(&cmd_info);
}

void cli_reg_cmd_apps(void)
{
    static const struct cli_command cmd_info = {"app", "app info", cmd_apps_func};

    aos_cli_register_command(&cmd_info);

    test_register_system_cmd();
}
