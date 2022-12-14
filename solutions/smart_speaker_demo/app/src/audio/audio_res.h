#ifndef _AUDIO_RES_H_
#define _AUDIO_RES_H_

#define LOCAL_AUDIO_HELLO_PATH           "/lfs/hello.wav"
#define LOCAL_AUDIO_NET_CFG_CONFIG_PATH  "/lfs/net_cfg_config.mp3"
#define LOCAL_AUDIO_NET_CFG_CONN_PATH    "/lfs/net_cfg_conn.mp3"
#define LOCAL_AUDIO_NET_CFG_FAIL_PATH    "/lfs/net_cfg_fail.mp3"
#define LOCAL_AUDIO_NET_CFG_START_PATH   "/lfs/net_cfg_start.mp3"
#define LOCAL_AUDIO_NET_CFG_SWITCH_PATH  "/lfs/net_cfg_switch.mp3"
#define LOCAL_AUDIO_NET_CFG_TIMEOUT_PATH "/lfs/net_cfg_timeout.mp3"
#define LOCAL_AUDIO_NET_FAIL_PATH        "/lfs/net_fail.mp3"
#define LOCAL_AUDIO_NET_SUCC_PATH        "/lfs/net_succ.mp3"
#define LOCAL_AUDIO_OK_PATH              "/lfs/ok.mp3"
#define LOCAL_AUDIO_PLAY_ERR_PATH        "/lfs/play_err.mp3"
#define LOCAL_AUDIO_SORRY_PATH           "/lfs/sorry.mp3"
#define LOCAL_AUDIO_SORRY2_PATH          "/lfs/sorry2.mp3"
#define LOCAL_AUDIO_STARTING_PATH        "/lfs/starting.mp3"

typedef enum {
    LOCAL_AUDIO_HELLO,
    LOCAL_AUDIO_NET_CFG_CONFIG,
    LOCAL_AUDIO_NET_CFG_CONN,
    LOCAL_AUDIO_NET_CFG_FAIL,
    LOCAL_AUDIO_NET_CFG_START,
    LOCAL_AUDIO_NET_CFG_SWITCH,
    LOCAL_AUDIO_NET_CFG_TIMEOUT,
    LOCAL_AUDIO_NET_FAIL,
    LOCAL_AUDIO_NET_SUCC,
    LOCAL_AUDIO_OK,
    LOCAL_AUDIO_PLAY_ERR,
    LOCAL_AUDIO_SORRY,
    LOCAL_AUDIO_SORRY2,
    LOCAL_AUDIO_STARTING,
    LOCAL_AUDIO_END
} local_audio_name_t;
int local_audio_play(local_audio_name_t name);

#define AUDIO_RES_ARRAY \
{LOCAL_AUDIO_HELLO_PATH}, \
{LOCAL_AUDIO_NET_CFG_CONFIG_PATH}, \
{LOCAL_AUDIO_NET_CFG_CONN_PATH}, \
{LOCAL_AUDIO_NET_CFG_FAIL_PATH}, \
{LOCAL_AUDIO_NET_CFG_START_PATH}, \
{LOCAL_AUDIO_NET_CFG_SWITCH_PATH}, \
{LOCAL_AUDIO_NET_CFG_TIMEOUT_PATH}, \
{LOCAL_AUDIO_NET_FAIL_PATH}, \
{LOCAL_AUDIO_NET_SUCC_PATH}, \
{LOCAL_AUDIO_OK_PATH}, \
{LOCAL_AUDIO_PLAY_ERR_PATH}, \
{LOCAL_AUDIO_SORRY_PATH}, \
{LOCAL_AUDIO_SORRY2_PATH}, \
{LOCAL_AUDIO_STARTING_PATH}, \
{NULL}
#endif
