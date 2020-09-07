/*
 * Copyright (C) 2018-2020 Alibaba Group Holding Limited
 */

#ifndef __LC_FIFO_H__
#define __LC_FIFO_H__

#include <aos/types.h>
#include <aos/kernel.h>
#include <aos/aos.h>
#include <aos/ringbuffer.h>

__BEGIN_DECLS__

typedef struct lc_fifo {
    char               *buf;
    dev_ringbuf_t       rb;
    aos_mutex_t        lock;
} lcfifo_t;

/**
 * @brief  create circle fifo with lock
 * @param  [in] len : length of the circle fifo
 * @return
 */
lcfifo_t* lcfifo_create(size_t len);

/**
 * @brief  destroy the lcfifo
 * @param  [in] fifo
 * @return
 */
int lcfifo_destroy(lcfifo_t* fifo);

/**
 * @brief  read
 * @param  [in] fifo
 * @param  [in] data
 * @param  [in] size
 * @return real read size
 */
int lcfifo_read(lcfifo_t* fifo, uint8_t *data, size_t size);

/**
 * @brief  write
 * @param  [in] fifo
 * @param  [in] data
 * @param  [in] size
 * @return real write size
 */
int lcfifo_write(lcfifo_t* fifo, const uint8_t *data, size_t size);

/**
 * @brief  is full of the fifo
 * @param  [in] fifo
 * @return
 */
int lcfifo_is_full(lcfifo_t* fifo);

/**
 * @brief  is empty of the fifo
 * @param  [in] fifo
 * @return
 */
int lcfifo_is_empty(lcfifo_t* fifo);

/**
 * @brief  get avaliable read len
 * @param  [in] fifo
 * @return
 */
int lcfifo_get_rlen(lcfifo_t* fifo);

/**
 * @brief  get avaliable write len
 * @param  [in] fifo
 * @return
 */
int lcfifo_get_wlen(lcfifo_t* fifo);

__END_DECLS__

#endif /* __LC_FIFO_H__ */

