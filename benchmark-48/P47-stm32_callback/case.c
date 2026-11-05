/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2016 Linaro Limited.
 * Copyright (c) 2024 STMicroelectronics
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
int c, d, k, l, r, enabled, bad;

struct device {
    void *data;
};

typedef void (*uart_irq_callback_user_data_t)(const struct device *, void *);

struct payload {
    int id;
} old_data = {1}, new_data = {2};

struct uart_stm32_data {
    uart_irq_callback_user_data_t user_cb;
    void *user_data;
} uart;
static struct device dev = {&uart};

void old_callback(const struct device *dev, void *ptr)
{
    (void)dev;
    c++;
    if (((struct payload *)ptr)->id != 1)
        bad = 1;
}

void new_callback(const struct device *dev, void *ptr)
{
    (void)dev;
    c++;
    if (((struct payload *)ptr)->id != 2)
        bad = 1;
}

#define a (uart.user_cb == old_callback ? 1 : uart.user_cb == new_callback ? 2 : 0)
#define b (((struct payload *)uart.user_data)->id)

unsigned irq_lock(void)
{
    unsigned key = enabled;
    enabled = 0;
    return key;
}

void irq_unlock(unsigned key)
{
    enabled = key;
}

void initialize(void)
{
    c = d = k = l = r = bad = 0;
    enabled = 1;
    uart.user_cb = old_callback;
    uart.user_data = &old_data;
}

void device_irq(void)
{
    uart.user_cb(&dev, uart.user_data);
}

static void uart_stm32_irq_callback_set(const struct device *dev, uart_irq_callback_user_data_t cb,
                                        void *cb_data)
{
    struct uart_stm32_data *data = dev->data;
    unsigned int key;
    STEP(0, {
        key = irq_lock();
        k = (int)key;
    });

    STEP(1, { data->user_cb = cb; });
    STEP(2, { data->user_data = cb_data; });

#if defined(CONFIG_UART_EXCLUSIVE_API_CALLBACKS)
    data->async_cb = NULL;
    data->async_user_data = NULL;
#endif

    STEP(3, { irq_unlock(key); });
}

void foreground(void)
{
    uart_stm32_irq_callback_set(&dev, new_callback, &new_data);
}
