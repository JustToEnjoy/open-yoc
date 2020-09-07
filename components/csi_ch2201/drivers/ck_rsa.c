/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     ck_rsa.c
 * @brief    CSI Source File for RSA Driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "drv/rsa.h"
#include "ck_rsa.h"

#define ERR_RSA(errno) (CSI_DRV_ERRNO_RSA_BASE | errno)
#define RSA_NULL_PARAM_CHK(para)  HANDLE_PARAM_CHK(para, ERR_RSA(DRV_ERROR_PARAMETER))

typedef struct {
#ifdef CONFIG_LPM
    uint8_t rsa_power_status;
    uint32_t rsa_regs_saved[4];
#endif
    uint32_t base;
    uint32_t irq;
    rsa_event_cb_t cb;
    rsa_data_bits_e data_bit;
    rsa_endian_mode_e endian;
    rsa_padding_t padding;
    rsa_status_t status;
} ck_rsa_priv_t;

extern int32_t target_get_rsa_count(void);
extern int32_t target_get_rsa(int32_t idx, uint32_t *base, uint32_t *irq);

static ck_rsa_priv_t rsa_handle[CONFIG_RSA_NUM];

extern uint8_t modulus[];
static ck_rsa_reg_t *rsa_reg = NULL;
static uint32_t g_acc[RSA_KEY_WORD];
/* Driver Capabilities */
static const rsa_capabilities_t driver_capabilities = {
    .bits_192 = 1, /* 192bits modular mode */
    .bits_256 = 1, /* 256bits modular mode */
    .bits_512 = 1, /* 512bits modular mode */
    .bits_1024 = 1, /* 1024bits modular mode */
    .bits_2048 = 1, /* 2048bits modular mode */
    .bits_3072 = 0  /* 3072bits modular mode */
};

//
// Functions
//

static uint32_t sw_exptmod_2_2m(const uint32_t *modulus, uint32_t words, uint32_t *tmp_c);
static uint32_t get_valid_bits(const uint32_t *addr, uint32_t wordsize, uint32_t keywords);

static uint32_t g_acc[RSA_KEY_WORD];

static inline void rsa_clear_int(void)
{
    rsa_reg->RSA_ISR = 0xffff;
    rsa_reg->RSA_IMR = 0x0000;
}

static inline void rsa_setm_width(uint32_t width)
{
    rsa_reg->RSA_MWID = width;
}

static inline void rsa_setd_width(uint32_t width)
{
    rsa_reg->RSA_CKID = width;
}

static inline void rsa_setb_width(uint32_t width)
{
    rsa_reg->RSA_BWID = width;
}

static inline void rsa_cal_q(void)
{
    rsa_reg->RSA_CTRL = RAS_CALCULATE_Q;
}

static inline void rsa_opr_start(void)
{
    rsa_reg->RSA_CTRL = RSA_ENABLE_MODULE;
}

static inline int32_t rsa_opr_reset(void)
{
    rsa_reg->RSA_CTRL = RSA_ENDIAN_MODE;
    rsa_reg->RSA_RST |= RSA_RESET;
    volatile uint32_t timeout = 0x1000000;

    while (rsa_reg->RSA_RST) {
        if (!(timeout--)) {
            return ERR_RSA(DRV_ERROR_TIMEOUT);
        }
    }

    return 0;
}

static inline uint32_t rsa_loop_cnt(void)
{
    return rsa_reg->RSA_LP_CNT;
}

static inline uint32_t rsa_cal_q_done(void)
{
    return (rsa_reg->RSA_ISR >> RSA_CAL_Q_DONE_OFFSET) & 0x1;
}

static inline uint32_t rsa_opr_done(void)
{
    return (rsa_reg->RSA_ISR) & 0x1;
}

static inline uint32_t rsa_raise_exception(void)
{
    return (rsa_reg->RSA_ISR) & 0x1E;
}

static inline uint32_t rsa_loadm(uint32_t *data, uint32_t length)
{
    uint32_t i;
    uint32_t baseaddr = (uint32_t)&rsa_reg->RSA_RFM;

    for (i = 0; i < length; i++) {
        *(volatile uint32_t *)baseaddr = data[i];
        baseaddr = baseaddr + 4;
    }

    return 0;
}

static void rsa_loadd(uint32_t *data, uint32_t length)
{
    uint32_t i;
    uint32_t baseaddr = (uint32_t)&rsa_reg->RSA_RFD;

    for (i = 0;  i < length; i++) {
        *(volatile uint32_t *)baseaddr = data[i];
        baseaddr = baseaddr + 4;
    }
}

static void rsa_loadc(uint32_t *data, uint32_t length)
{
    uint32_t i;
    uint32_t baseaddr = (uint32_t)&rsa_reg->RSA_RFC;

    for (i = 1; i < length + 1; i++) {
        *(volatile uint32_t *)baseaddr = data[i - 1];
        baseaddr = baseaddr + 4;
    }
}

static void rsa_loadb(uint32_t *data, uint32_t length)
{
    uint32_t i;
    uint32_t baseaddr = (uint32_t)&rsa_reg->RSA_RFB;

    for (i = 0; i < length; i++) {
        *(volatile uint32_t *)baseaddr = data[i];
        baseaddr = baseaddr + 4;
    }
}

static void rsa_read_r(uint32_t data[], uint32_t length)
{
    uint32_t i;
    uint32_t baseaddr = (uint32_t)&rsa_reg->RSA_RFR;

    for (i = 0; i < length; i++) {
        data[i] = *(uint32_t *)baseaddr;
        baseaddr = baseaddr + 4;
    }
}

static uint32_t rsa_exptmod(const uint32_t *modulus, const uint32_t *exponent,
                            const uint32_t *base, uint32_t *out, uint32_t keywords)
{
#ifndef RSA_USING_ID2KEY
    uint32_t tmp_c[RSA_KEY_WORD];
#endif
    uint32_t ret = 0;
    register uint32_t timeout = 0x1000000;

    if ((NULL == exponent) || (NULL == base) || (NULL == out)) {
        return 1;
    }

#ifndef RSA_USING_ID2KEY

    if (g_acc[0] != 0x0) {
        memcpy(tmp_c, g_acc, sizeof(g_acc));
    } else {
        ret = sw_exptmod_2_2m(modulus, keywords, tmp_c);

        if (ret != 0) {
            return ret;
        }
    }

#endif

    /* reset for safe */
    ret = rsa_opr_reset();

    if (ret) {
        return ret;
    }

    /* clear and disable int */
    rsa_clear_int();
    /* set m */
    rsa_setm_width(keywords >> 1);
    rsa_loadm((uint32_t *)modulus, keywords);
    /* set d */
    rsa_setd_width(get_valid_bits(exponent, keywords, keywords) - 1);
    rsa_loadd((uint32_t *)exponent, keywords);
    /* set b */
    rsa_setb_width(keywords >> 1);
    rsa_loadb((uint32_t *)base, keywords);
    /* set c */
#ifndef RSA_USING_ID2KEY
    rsa_loadc(tmp_c, keywords);
#else
    rsa_loadc(g_acc, keywords);
#endif
        rsa_cal_q();

        while ((!rsa_cal_q_done() && (!rsa_raise_exception()))) {
            if (!(timeout--)) {
                return ERR_RSA(DRV_ERROR_TIMEOUT);
            }
        }

    if (!rsa_raise_exception()) {
            rsa_opr_start();

            while ((!rsa_opr_done()) && (rsa_loop_cnt() < MAX_RSA_LP_CNT) && (!rsa_raise_exception()));

        if ((rsa_loop_cnt() >= MAX_RSA_LP_CNT)
            || rsa_raise_exception()) {
            ret = 1;
        } else {
            rsa_read_r(out, keywords);
        }
    } else {
        ret = 1;
    }

    if (ret) {
        return ret;
    }

    ret = rsa_opr_reset();

    return ret;

}

static uint32_t get_valid_bits(const uint32_t *addr, uint32_t wordsize, uint32_t keywords)
{
    uint32_t i = 0;
    uint32_t j = 0;

    for (i = wordsize; i > 0; i--) {
        if (addr[i - 1]) {
            break;
        }
    }

    for (j = keywords; j > 0; j--) {
        if (addr[i - 1] & (0x1 << (j - 1))) {
            break;
        }
    }

    return ((i - 1) << 5) + j;
}

static uint32_t get_first_nonzero_words(uint32_t *a, uint32_t max_words)
{
    uint32_t i = 0;

    for (i = max_words; i > 0; i--) {
        if (a[i - 1]) {
            return i;
        }
    }

    return 0;
}

static uint32_t word_array_left_shift(uint32_t *a, uint32_t words,
                                      uint32_t shift_bits, uint32_t *r)
{
    uint32_t i = 0;
    uint32_t w = shift_bits >> 5;
    uint32_t b = shift_bits - (w << 5);

    for (i = 0; i < w; i++) {
        r[i] = 0;
    }

    uint32_t tmp = 0;

    for (i = 0; i < words; i++) {
        r[w + i] = (tmp | ((a[i] << b) & (~((0x1 << b) - 1))));
        tmp = ((a[i] >> (32 - b)) & ((0x1 << b) - 1));
    }

    r[w + i] = tmp;

    return 0;
}

/* r = a - b */
static uint32_t _word_array_sub(uint32_t *a, uint32_t a_words,
                                uint32_t *b, uint32_t b_words,
                                uint32_t *r)
{
    uint32_t i;
    uint64_t tmp = 0;
    uint32_t borrow = 0;

    for (i = 0; i < b_words; i++) {
        tmp = UINT32_TO_UINT64(a[i]) - UINT32_TO_UINT64(b[i]) - UINT32_TO_UINT64(borrow);
        r[i] = UINT64L_TO_UINT32(tmp);
        borrow = ((UINT64H_TO_UINT32(tmp) == 0) ? (0) : (0xffffffff - UINT64H_TO_UINT32(tmp) + 1));
    }

    for (i = b_words; i < a_words; i++) {
        tmp = UINT32_TO_UINT64(a[i]) - UINT32_TO_UINT64(borrow);
        r[i] = UINT64L_TO_UINT32(tmp);
        borrow = ((UINT64H_TO_UINT32(tmp) == 0) ? (0) : (0xffffffff - UINT64H_TO_UINT32(tmp) + 1));
    }

    if (borrow) {
        return -1;
    }

    return 0;
}

static uint32_t word_array_mod(uint32_t *a, uint32_t a_words,
                               uint32_t *b, uint32_t b_words,
                               uint32_t *r, uint32_t keywords)
{
    uint32_t ret;
    bignum_t tmpa;
    bignum_t tmpb;

    memset(&tmpa, 0, sizeof(tmpa));
    memset(&tmpb, 0, sizeof(tmpa));

    uint32_t b_valid_bits = get_valid_bits(b, b_words, keywords);

    memcpy(tmpa.pdata, a, (a_words << 2));

    do {
        uint32_t tmpa_words = get_first_nonzero_words(tmpa.pdata, a_words);
        uint32_t tmpa_valid_bits = get_valid_bits(tmpa.pdata, tmpa_words, keywords);

        if (tmpa_valid_bits > b_valid_bits + 1) {
            memset(tmpb.pdata, 0, (a_words << 2));
            word_array_left_shift(b, b_words, tmpa_valid_bits - b_valid_bits - 1,
                                  tmpb.pdata);
            uint32_t tmpb_words = get_first_nonzero_words(tmpb.pdata, a_words);
            ret = _word_array_sub(tmpa.pdata, tmpa_words, tmpb.pdata, tmpb_words, tmpa.pdata);
        } else if (tmpa_words == b_words) {
            memcpy(r, tmpa.pdata, (tmpa_words << 2));
            ret = _word_array_sub(r, tmpa_words, b, b_words, tmpa.pdata);
        } else {
            ret = _word_array_sub(tmpa.pdata, tmpa_words, b, b_words, tmpa.pdata);
        }
    } while (ret == 0);

    return 0;
}

static uint32_t sw_exptmod_2_2m(const uint32_t *modulus, uint32_t words, uint32_t *tmp_c)
{
    bignum_t tmp;

    memset(&tmp, 0, sizeof(bignum_t));

    uint32_t m_valid_bits = (words << 5);

    uint32_t data1 = 0x1;
    word_array_left_shift(&data1, 1, (m_valid_bits << 1), tmp.pdata);
    tmp.words = get_first_nonzero_words(tmp.pdata, words * 2 + 1);

    uint32_t ret = word_array_mod(tmp.pdata, tmp.words,
                                  (uint32_t *)modulus, words, tmp_c, words);

    if (ret != 0) {
        return ret;
    }

    return 0;
}

static void convert_byte_array(uint8_t *in, uint8_t *out, uint32_t len)
{
    uint32_t idx, round = len >> 1;

    for (idx = 0; idx < round; idx++) {
        uint8_t tmp = *(in + idx);
        *(out + idx) = *(in + len - 1 - idx);
        *(out + len - 1 - idx) = tmp;
    }

    if (len & 0x1) {
        *(out + round) = *(in + round);
    }
}

static void convert_buf_to_bndata(const uint8_t *src, uint32_t src_bytes,
                                  uint32_t *dst, uint32_t dst_words)
{
    memset(dst, 0, dst_words << 2);
    convert_byte_array((uint8_t *)src, (uint8_t *)dst, src_bytes);
}

static void convert_bndata_to_buf(const uint32_t *src, uint32_t src_words,
                                  uint8_t *dst, uint32_t dst_bytes)
{
    memset(dst, 0, dst_bytes);
    convert_byte_array((uint8_t *)src, (uint8_t *)dst, dst_bytes);
}

static const uint8_t der_sha256_t[] = {
    0x30, 0x31,
    0x30, 0x0d,
    0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, /* id-sha256 */
    0x05, 0x00,
    0x04, 0x20
};

static const uint8_t der_sha1_t[] = {
    0x30, 0x21,
    0x30, 0x09,
    0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a,
    0x05, 0x00,
    0x04, 0x14
};

static const uint8_t der_md5_t[] = {
    0x30, 0x20, /* type Sequence, length 0x20 (32) */
    0x30, 0x0c, /* type Sequence, length 0x09 */
    0x06, 0x08, /* type OID, length 0x05 */
    0x2a, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x02, 0x05, /* id-md5 */
    0x05, 0x00, /* NULL */
    0x04, 0x10  /* Octet string, length 0x10 (16), followed by md5 hash */
};

static uint32_t rsa_padding_pkcs(uint8_t  *dgst,
                                 uint8_t  *out,
                                 uint32_t  type,
                                 uint32_t  keybytes)

{
    uint32_t i;
    uint8_t *p;
    uint8_t *der;
    uint32_t der_len;
    uint32_t hashlen;
    uint32_t pslen;

    if (type == MD5_PADDING) {
        der     = (uint8_t *)der_md5_t;
        der_len = sizeof(der_md5_t);
        hashlen = MD5_HASH_SZ;
    } else if (type == SHA256_PADDING) {
        der     = (uint8_t *)der_sha256_t;
        der_len = sizeof(der_sha256_t);
        hashlen = SHA256_HASH_SZ;
    } else {
        der     = (uint8_t *)der_sha1_t;
        der_len = sizeof(der_sha1_t);
        hashlen = SHA1_HASH_SZ;
    }

    p = (uint8_t *)out;

    *(p++) = 0x00;
    *(p++) = 0x01;

    /* pad out with 0xff data */
    pslen = keybytes - 3 - der_len - hashlen;

    for (i = 0; i < pslen; i++) {
        p[i] = 0xff; /* PS */
    }

    p += pslen;
    *(p++) = 0x0;

    for (i = 0; i < der_len; i++) {
        p[i] = der[i];
    }

    p += der_len;

    for (i = 0; i < hashlen; i++) {
        p[i] = dgst[i];
    }

    return 0;
}

static uint32_t rsa_checking_pkcs(uint8_t *dgst,
                                  uint8_t *in,
                                  uint32_t inlen,
                                  uint8_t *is_valid,
                                  uint32_t type,
                                  uint32_t keybytes)
{
    uint32_t i;
    uint32_t ret;
    const uint8_t *p;
    uint8_t *der = (uint8_t *)der_sha1_t;
    uint32_t der_len = sizeof(der_sha1_t);
    uint32_t hashlen = SHA1_HASH_SZ;
    uint32_t pslen;

    if (type == MD5_PADDING) {
        der     = (uint8_t *)der_md5_t;
        der_len = sizeof(der_md5_t);
        hashlen = MD5_HASH_SZ;
    } else if (type == SHA1_PADDING) {
        der     = (uint8_t *)der_sha1_t;
        der_len = sizeof(der_sha1_t);
        hashlen = SHA1_HASH_SZ;
    } else if (type == SHA256_PADDING) {
        der     = (uint8_t *)der_sha256_t;
        der_len = sizeof(der_sha256_t);
        hashlen = SHA256_HASH_SZ;
    }

    *is_valid = 0;

    pslen = keybytes - 3 - der_len - hashlen;
    p = in;
    p++;

    if (*(p) != 0x01) {
        ret = -1;
        goto _verify_fail;
    }

    p++;

    /* scan PS */
    for (i = 0; i < pslen; i++) {
        if (*(p + i) != 0xff) {
            ret = -2;
            goto _verify_fail;
        }
    }

    p += pslen;

    if ((*p) != 0x00) {
        ret = -1;
        goto _verify_fail;
    }

    p++;

    /* scan t */
    for (i = 0; i < der_len; i++) {
        if (*(p + i) != der[i]) {
            ret = -3;
            goto _verify_fail;
        }
    }

    p += der_len;

    for (i = 0; i < hashlen; i++) {
        if (*(p + i) != dgst[i]) {
            ret = -4;
            goto _verify_fail;
        }
    }

    *is_valid = 1;
    ret = 0;

_verify_fail:

    return ret;
}

static uint32_t rsa_padding_es_pkcs(uint8_t *dgst,
                                    uint32_t dgstlen,
                                    uint8_t *out,
                                    uint32_t padding,
                                    uint32_t keybytes)

{
    uint32_t i;
    uint8_t *p;
    uint32_t pslen;

    p = (uint8_t *)out;

    *(p++) = 0x00;
    *(p++) = 0x02;

    /* pad out with 0xff data */
    pslen = keybytes - 3 - dgstlen;

    for (i = 0; i < pslen; i++) {
        p[i] = 0xff; /* PS */
    }

    p += pslen;
    *(p++) = 0x0;

    for (i = 0; i < dgstlen; i++) {
        p[i] = dgst[i];
    }

    return 0;
}

static uint32_t rsa_checking_es_pkcs(uint8_t *out,
                                     uint32_t *out_size,
                                     uint8_t *src,
                                     uint32_t src_size,
                                     uint32_t padding,
                                     uint32_t keybytes)
{
    uint32_t i;
    uint8_t *p;
    uint8_t *p_src;
    uint32_t pslen;

    p = (uint8_t *)src;
    p_src = p;
    *(p++) = 0x00;

    if (padding == PKCS1_PADDING) {
        if (*(p++) != 0x02) {
            return -1;
        }
    } else {
        if (*(p++) != 0x01) {
            return -2;
        }
    }

    pslen = src_size - 2;

    while (pslen--) {
        if (*(p++) == 0x0) {
            break;
        }
    }

    if (padding == PKCS1_PADDING) {
        *out_size = pslen;
    } else {
        *out_size = keybytes;
    }

    for (i = 0; i < *out_size; i++) {
        if (padding == PKCS1_PADDING) {
            out[i] = p[i];
        } else {
            out[i] = p_src[i];
        }
    }

    return 0;
}

int rsa_encrypt(uint8_t *n,         uint8_t *e,
                uint8_t *src,       uint32_t src_size,
                uint8_t *out,       uint32_t *out_size,
                uint32_t padding,   uint32_t keybits_len)
{
    uint32_t ret;
    uint32_t tmp_n[RSA_KEY_WORD];
    uint32_t tmp_e[RSA_KEY_WORD];
    uint32_t tmp_in_out[RSA_KEY_WORD];
    uint32_t keywords = 0, keybytes = 0;

    keywords = GET_KEY_WORD(keybits_len);
    keybytes = GET_KEY_BYTE(keybits_len);

    convert_buf_to_bndata(n, keybytes, tmp_n, keywords);
    convert_buf_to_bndata(e, keybytes, tmp_e, keywords);

    if (padding == PKCS1_PADDING) {
        ret = rsa_padding_es_pkcs(src,
                                  src_size,
                                  (uint8_t *)tmp_in_out,
                                  padding,
                                  keybytes);

        if (ret != 0) {
            return ret;
        }

        convert_byte_array((uint8_t *)tmp_in_out, (uint8_t *)tmp_in_out, keybytes);
    } else {
        convert_byte_array((uint8_t *)src, (uint8_t *)tmp_in_out, keybytes);
    }

    ret = rsa_exptmod(tmp_n, tmp_e, tmp_in_out, tmp_in_out, keywords);

    if (ret != 0) {
        return ret;
    }

    convert_bndata_to_buf(tmp_in_out, keywords, out, keybytes);
    *out_size = keybytes;
    return ret;
}

int rsa_decrypt(uint8_t *n,         uint8_t *d,
                uint8_t *src,       uint32_t src_size,
                uint8_t *out,       uint32_t *out_size,
                uint32_t padding,   uint32_t keybits_len)
{
    uint32_t ret;
    uint32_t tmp_n[RSA_KEY_WORD];
    uint32_t tmp_d[RSA_KEY_WORD];
    uint32_t tmp_in_out[RSA_KEY_WORD];
    uint32_t keywords = 0, keybytes = 0;

    keywords = GET_KEY_WORD(keybits_len);
    keybytes = GET_KEY_BYTE(keybits_len);

    convert_buf_to_bndata(n, keybytes, tmp_n, keywords);
    convert_buf_to_bndata(d, keybytes, tmp_d, keywords);
    convert_buf_to_bndata(src, src_size, tmp_in_out, keywords);

    ret = rsa_exptmod(tmp_n, tmp_d, tmp_in_out, tmp_in_out, keywords);

    if (ret != 0) {
        return ret;
    }

    convert_byte_array((uint8_t *)tmp_in_out, (uint8_t *)tmp_in_out, keybytes);

    ret = rsa_checking_es_pkcs(out,
                               out_size,
                               (uint8_t *)tmp_in_out,
                               keybytes,
                               padding,
                               keybytes);

    return ret;
}

int rsa_sign(uint8_t *n,        uint8_t *d,
             uint8_t *src,      uint32_t src_size,
             uint8_t *signature, uint32_t *sig_size,
             uint32_t type,     uint32_t keybits_len)
{
    uint32_t ret;
    uint32_t tmp_n[RSA_KEY_WORD];
    uint32_t tmp_d[RSA_KEY_WORD];
    uint32_t tmp_in_out[RSA_KEY_WORD];
    uint32_t keywords = 0, keybytes = 0;

    keywords = GET_KEY_WORD(keybits_len);
    keybytes = GET_KEY_BYTE(keybits_len);

    convert_buf_to_bndata(n, keybytes, tmp_n, keywords);
    convert_buf_to_bndata(d, keybytes, tmp_d, keywords);

    ret = rsa_padding_pkcs(src,
                           (uint8_t *)tmp_in_out,
                           type,
                           keybytes);

    if (ret != 0) {
        return ret;
    }

    convert_byte_array((uint8_t *)tmp_in_out, (uint8_t *)tmp_in_out, keybytes);

    ret = rsa_exptmod(tmp_n, tmp_d, tmp_in_out, tmp_in_out, keywords);

    if (ret != 0) {
        return ret;
    }

    convert_bndata_to_buf(tmp_in_out, keywords, signature, keybytes);
    *sig_size = keybytes;

    return 0;
}

int rsa_verify(uint8_t *n,          uint8_t *e,
               uint8_t *src,        uint32_t src_size,
               uint8_t *signature,  uint32_t sig_size,
               uint32_t type,       uint32_t keybits_len,
               uint8_t *result)
{
    uint32_t ret;
    uint32_t tmp_n[RSA_KEY_WORD];
    uint32_t tmp_e[RSA_KEY_WORD];
    uint32_t tmp_in_out[RSA_KEY_WORD];
    uint32_t keywords = 0, keybytes = 0;

    *result = 0;

    keywords = GET_KEY_WORD(keybits_len);
    keybytes = GET_KEY_BYTE(keybits_len);

    convert_buf_to_bndata(n, keybytes, tmp_n, keywords);
    convert_buf_to_bndata(e, keybytes, tmp_e, keywords);
    convert_buf_to_bndata(signature, sig_size, tmp_in_out, keywords);

    ret = rsa_exptmod(tmp_n, tmp_e, tmp_in_out, tmp_in_out, keywords);

    if (ret != 0) {
        return ret;
    }

    convert_byte_array((uint8_t *)tmp_in_out, (uint8_t *)tmp_in_out, keybytes);

    ret = rsa_checking_pkcs(src,
                            (uint8_t *)tmp_in_out,
                            keybytes,
                            result,
                            type,
                            keybytes);

    return ret;
}

static int rsa_sw_exptmod_2_2m(uint8_t *modulus, uint32_t keybits_len)
{
#ifdef RSA_USING_ID2KEY
    uint32_t keywords = 0, keybytes = 0;
    uint32_t tmp_n[RSA_KEY_WORD];

    keywords = GET_KEY_WORD(keybits_len);
    keybytes = GET_KEY_BYTE(keybits_len);

    convert_buf_to_bndata(modulus, keybytes, tmp_n, keywords);

    sw_exptmod_2_2m(tmp_n, keywords, g_acc);
#endif
    return 0;
}

int rsa_sw_calc_modulus(uint8_t *modulus, uint32_t keybits_len)
{
    static uint32_t current_keybits_len;

    if (current_keybits_len != keybits_len) {
        rsa_sw_exptmod_2_2m((uint8_t *)modulus, keybits_len);
        current_keybits_len = keybits_len;
    }

    return 0;
}

#ifdef CONFIG_LPM
static void manage_clock(rsa_handle_t handle, uint8_t enable)
{
    drv_clock_manager_config(CLOCK_MANAGER_RSA, enable);
}

static void do_prepare_sleep_action(rsa_handle_t handle)
{
    ck_rsa_priv_t *rsa_priv = handle;
    uint32_t *rbase = (uint32_t *)(rsa_priv->base);
    registers_save(rsa_priv->rsa_regs_saved, rbase, 4);
}

static void do_wakeup_sleep_action(rsa_handle_t handle)
{
    ck_rsa_priv_t *rsa_priv = handle;
    uint32_t *rbase = (uint32_t *)(rsa_priv->base);
    registers_restore(rbase, rsa_priv->rsa_regs_saved, 4);
}
#endif

/**
  \brief       Initialize RSA Interface. 1. Initializes the resources needed for the RSA interface 2.registers event callback function
  \param[in]   idx  index of rsa
  \param[in]   cb_event  Pointer to \ref rsa_event_cb_t
  \return      pointer to rsa handle
*/
rsa_handle_t csi_rsa_initialize(int32_t idx, rsa_event_cb_t cb_event)
{
    if (idx < 0 || idx >= CONFIG_RSA_NUM) {
        return NULL;
    }

    /* obtain the rsa information */
    uint32_t base = 0u;
    uint32_t irq;
    int32_t real_idx = target_get_rsa(idx, &base, &irq);

    if (real_idx != idx) {
        return NULL;
    }

    ck_rsa_priv_t *rsa_priv = &rsa_handle[idx];

    rsa_priv->base = base;
    rsa_priv->irq  = irq;

    /* initialize the rsa context */
    rsa_priv->cb = cb_event;
    rsa_priv->data_bit = RSA_DATA_BITS_1024;
    rsa_priv->endian = RSA_ENDIAN_MODE_LITTLE;
    rsa_priv->padding.padding_type = RSA_PADDING_MODE_PKCS1;
    rsa_priv->padding.hash_type = RSA_HASH_TYPE_SHA1;
    rsa_priv->status.busy = 0;

    memset(g_acc, 0x0, sizeof(g_acc));
#ifdef CONFIG_LPM
    csi_rsa_power_control(rsa_priv, DRV_POWER_FULL);
#endif

    return (rsa_handle_t)rsa_priv;
}

/**
  \brief       De-initialize RSA Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle  rsa handle to operate.
  \return      error code
*/
int32_t csi_rsa_uninitialize(rsa_handle_t handle)
{
    RSA_NULL_PARAM_CHK(handle);

    ck_rsa_priv_t *rsa_priv = handle;
    rsa_priv->cb = NULL;

#ifdef CONFIG_LPM
    csi_rsa_power_control(rsa_priv, DRV_POWER_OFF);
#endif
    return 0;
}

/**
  \brief       control rsa power.
  \param[in]   handle  rsa handle to operate.
  \param[in]   state   power state.\ref csi_power_stat_e.
  \return      error code
*/
int32_t csi_rsa_power_control(rsa_handle_t handle, csi_power_stat_e state)
{
    RSA_NULL_PARAM_CHK(handle);

#ifdef CONFIG_LPM
    power_cb_t callback = {
        .wakeup = do_wakeup_sleep_action,
        .sleep = do_prepare_sleep_action,
        .manage_clock = manage_clock
    };
    return drv_soc_power_control(handle, state, &callback);
#else
    return ERR_RSA(DRV_ERROR_UNSUPPORTED);
#endif
}

/**
  \brief       Get driver capabilities.
  \param[in]   idx device id
  \return      \ref rsa_capabilities_t
*/
rsa_capabilities_t csi_rsa_get_capabilities(int32_t idx)
{
    if (idx < 0 || idx >= CONFIG_RSA_NUM) {
        rsa_capabilities_t ret;
        memset(&ret, 0, sizeof(rsa_capabilities_t));
        return ret;
    }

    return driver_capabilities;
}

/**
  \brief       config rsa mode.
  \param[in]   handle  rsa handle to operate.
  \param[in]   data_bits \ref rsa_data_bits_e
  \param[in]   endian    \ref rsa_endian_mode_e
  \param[in]   arg       the addr of modules value
  \return      error code
*/
int32_t csi_rsa_config(rsa_handle_t handle,
                       rsa_data_bits_e data_bits,
                       rsa_endian_mode_e endian,
                       void *arg
                      )
{
    RSA_NULL_PARAM_CHK(handle);

    ck_rsa_priv_t *rsa_priv = handle;
    rsa_reg = (ck_rsa_reg_t *)(rsa_priv->base);

    /* config the data bits */
    switch (data_bits) {
        case RSA_DATA_BITS_192:
        case RSA_DATA_BITS_256:
        case RSA_DATA_BITS_512:
            return ERR_RSA(DRV_ERROR_UNSUPPORTED);

        case RSA_DATA_BITS_1024:
        case RSA_DATA_BITS_2048:
            rsa_priv->data_bit = data_bits;
            break;

        default:
            return ERR_RSA(RSA_ERROR_DATA_BITS);
    }

    /* config the endian mode */
    if (endian == RSA_ENDIAN_MODE_LITTLE) {
        rsa_priv->endian = endian;
    } else if (endian == RSA_ENDIAN_MODE_BIG) {
        return ERR_RSA(DRV_ERROR_UNSUPPORTED);
    } else {
        return ERR_RSA(RSA_ERROR_ENDIAN);
    }

    if (arg != NULL) {
#ifdef RSA_USING_ID2KEY
        uint32_t keybits_len = 1024;

        if (data_bits == RSA_DATA_BITS_2048) {
            keybits_len = 2048;
        }

        rsa_sw_calc_modulus(arg, keybits_len);
#else
        memcpy(g_acc, arg, sizeof(g_acc));
#endif
    }

    return 0;
}

/**
  \brief       encrypt
  \param[in]   handle  rsa handle to operate.
  \param[in]   n         Pointer to the public modulus
  \param[in]   e         Pointer to the public exponent
  \param[in]   src       Pointer to the source data.
  \param[in]   src_size  the source data len
  \param[out]  out       Pointer to the result buffer
  \param[out]  out_size  the result size
  \param[in]   padding   \ref  rsa_padding_t
  \return      error code
*/
int32_t csi_rsa_encrypt(rsa_handle_t handle, void *n, void *e, void *src, uint32_t src_size, void *out, uint32_t *out_size, rsa_padding_t padding)
{
    RSA_NULL_PARAM_CHK(handle);
    RSA_NULL_PARAM_CHK(n);
    RSA_NULL_PARAM_CHK(e);
    RSA_NULL_PARAM_CHK(src);
    RSA_NULL_PARAM_CHK(out);
    RSA_NULL_PARAM_CHK(out_size);

    if (src_size <= 0 || (padding.hash_type != RSA_HASH_TYPE_MD5 && padding.hash_type != RSA_HASH_TYPE_SHA1 && padding.hash_type != RSA_HASH_TYPE_SHA256) || (padding.padding_type != RSA_PADDING_MODE_PKCS1)) {
        return ERR_RSA(DRV_ERROR_PARAMETER);
    }

    ck_rsa_priv_t *rsa_priv = handle;
    rsa_priv->status.busy = 1U;

    uint32_t bit_length = 1024;

    if (rsa_priv->data_bit == RSA_DATA_BITS_2048) {
        bit_length = 2048;
    }

    rsa_encrypt((uint8_t *)n, (uint8_t *)e, (uint8_t *)src, (uint32_t)src_size, (uint8_t *)out, (uint32_t *)out_size, (uint32_t)(padding.padding_type), bit_length);
    rsa_priv->status.busy = 0U;

    if (rsa_priv->cb) {
        rsa_priv->cb(0, RSA_EVENT_ENCRYPT_COMPLETE);
    }

    return 0;
}

/**
  \brief       decrypt
  \param[in]   handle  rsa handle to operate.
  \param[in]   n         Pointer to the public modulus
  \param[in]   d         Pointer to the privte exponent
  \param[in]   src       Pointer to the source data.
  \param[in]   src_size  the source data len
  \param[out]  out       Pointer to the result buffer
  \param[out]  out_size  the result size
  \param[in]   padding   \ref rsa_padding_t
  \return      error code
*/
int32_t csi_rsa_decrypt(rsa_handle_t handle, void *n, void *d, void *src, uint32_t src_size, void *out, uint32_t *out_size, rsa_padding_t padding)
{
    RSA_NULL_PARAM_CHK(handle);
    RSA_NULL_PARAM_CHK(n);
    RSA_NULL_PARAM_CHK(d);
    RSA_NULL_PARAM_CHK(src);
    RSA_NULL_PARAM_CHK(out);
    RSA_NULL_PARAM_CHK(out_size);

    if (src_size <= 0 || (padding.hash_type != RSA_HASH_TYPE_MD5 && padding.hash_type != RSA_HASH_TYPE_SHA1 && padding.hash_type != RSA_HASH_TYPE_SHA256) || (padding.padding_type != RSA_PADDING_MODE_PKCS1)) {
        return ERR_RSA(DRV_ERROR_PARAMETER);
    }

    ck_rsa_priv_t *rsa_priv = handle;
    rsa_priv->status.busy = 1U;

    uint32_t bit_length = 1024;

    if (rsa_priv->data_bit == RSA_DATA_BITS_2048) {
        bit_length = 2048;
    }

    rsa_decrypt((uint8_t *)n, (uint8_t *)d, (uint8_t *)src, (uint32_t)src_size, (uint8_t *)out, (uint32_t *)out_size, (uint32_t)(padding.padding_type), bit_length);
    rsa_priv->status.busy = 0U;

    if (rsa_priv->cb) {
        rsa_priv->cb(0, RSA_EVENT_DECRYPT_COMPLETE);
    }

    return 0;
}

/**
  \brief       rsa sign
  \param[in]   handle  rsa handle to operate.
  \param[in]   n         Pointer to the public modulus
  \param[in]   d         Pointer to the privte exponent
  \param[in]   src       Pointer to the source data.
  \param[in]   src_size  the source data len
  \param[out]  signature Pointer to the signature
  \param[out]  sig_size  the signature size
  \param[in]   padding   \ref rsa_padding_t
  \return      error code
*/
int32_t csi_rsa_sign(rsa_handle_t handle, void *n, void *d, void *src, uint32_t src_size, void *signature, uint32_t *sig_size, rsa_padding_t padding)
{
    RSA_NULL_PARAM_CHK(handle);
    RSA_NULL_PARAM_CHK(n);
    RSA_NULL_PARAM_CHK(d);
    RSA_NULL_PARAM_CHK(src);
    RSA_NULL_PARAM_CHK(signature);
    RSA_NULL_PARAM_CHK(sig_size);

    if (src_size <= 0 || (padding.hash_type != RSA_HASH_TYPE_MD5
                          && padding.hash_type != RSA_HASH_TYPE_SHA1
                          && padding.hash_type != RSA_HASH_TYPE_SHA256) || (padding.padding_type != RSA_PADDING_MODE_PKCS1)) {
        return ERR_RSA(DRV_ERROR_PARAMETER);
    }

    ck_rsa_priv_t *rsa_priv = handle;
    rsa_priv->status.busy = 1U;
    uint32_t bit_length = 1024;

    if (rsa_priv->data_bit == RSA_DATA_BITS_2048) {
        bit_length = 2048;
    }

    rsa_sign((uint8_t *)n, (uint8_t *)d, (uint8_t *)src, (uint32_t)src_size, (uint8_t *)signature, (uint32_t *)sig_size, (uint32_t)(padding.hash_type), bit_length);
    rsa_priv->status.busy = 0U;

    if (rsa_priv->cb) {
        rsa_priv->cb(0, RSA_EVENT_SIGN_COMPLETE);
    }

    return 0;
}

/**
  \brief       rsa verify
  \param[in]   handle  rsa handle to operate.
  \param[in]   n         Pointer to the public modulus
  \param[in]   e         Pointer to the public exponent
  \param[in]   src       Pointer to the source data.
  \param[in]   src_size  the source data len
  \param[in]   signature Pointer to the signature
  \param[in]   sig_size  the signature size
  \param[out]  result    Pointer to the result
  \param[in]   padding   \ref rsa_padding_t
  \return      error code
*/
int32_t csi_rsa_verify(rsa_handle_t handle, void *n, void *e, void *src, uint32_t src_size, void *signature, uint32_t sig_size, void *result, rsa_padding_t padding)
{
    RSA_NULL_PARAM_CHK(handle);
    RSA_NULL_PARAM_CHK(n);
    RSA_NULL_PARAM_CHK(e);
    RSA_NULL_PARAM_CHK(src);
    RSA_NULL_PARAM_CHK(signature);
    RSA_NULL_PARAM_CHK(result);

    if (src_size <= 0 || sig_size <= 0 || (padding.hash_type != RSA_HASH_TYPE_MD5 && padding.hash_type != RSA_HASH_TYPE_SHA1 && padding.hash_type != RSA_HASH_TYPE_SHA256) || (padding.padding_type != RSA_PADDING_MODE_PKCS1)) {
        return ERR_RSA(DRV_ERROR_PARAMETER);
    }

    ck_rsa_priv_t *rsa_priv = handle;
    rsa_priv->status.busy = 1U;

    uint32_t bit_length = 1024;

    if (rsa_priv->data_bit == RSA_DATA_BITS_2048) {
        bit_length = 2048;
    }

    rsa_verify((uint8_t *)n, (uint8_t *)e, (uint8_t *)src, (uint32_t)src_size, (uint8_t *)signature, sig_size, (uint32_t)(padding.hash_type), bit_length, (uint8_t *)result);
    rsa_priv->status.busy = 0U;

    if (rsa_priv->cb) {
        rsa_priv->cb(0, RSA_EVENT_VERIFY_COMPLETE);
    }

    return 0;
}

/**
  \brief       Get RSA status.
  \param[in]   handle  rsa handle to operate.
  \return      RSA status \ref rsa_status_t
*/
rsa_status_t csi_rsa_get_status(rsa_handle_t handle)
{
    if (handle == NULL) {
        rsa_status_t ret;
        memset(&ret, 0, sizeof(rsa_status_t));
        return ret;
    }

    ck_rsa_priv_t *rsa_priv = handle;
    return rsa_priv->status;
}
