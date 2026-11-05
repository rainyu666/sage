/*
 * Copyright (c) 2023 Antmicro <www.antmicro.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <errno.h>
int a, c, d, k, l, r, enabled, bad;

struct k_spinlock {
    int held;
};

typedef unsigned k_spinlock_key_t;

struct rzt2m_device_config {
    uintptr_t base;
} config = {5};

struct rzt2m_device_data {
    struct k_spinlock lock;
} driver;

struct device {
    const void *config;
    void *data;
} dev = {&config, &driver};

#define b driver.lock.held
#define BASE_TO_IFACE_ID(base) ((int)(base))
static uint32_t peripheral_register;
#define MSTPCRA (&peripheral_register)
#define MSTPCRA_MASK_SCIx(id) (1u << (id))
#define RDR(base) (&peripheral_register)
#define LOG_ERR(...) ((void)0)

unsigned irq_lock(void)
{
    unsigned key = enabled;
    enabled = 0;
    a++;
    return key;
}

void irq_unlock(unsigned key)
{
    a--;
    enabled = key;
}

k_spinlock_key_t k_spin_lock(struct k_spinlock *p)
{
    unsigned key = enabled;
    enabled = 0;
    p->held = 1;
    return key;
}

void k_spin_unlock(struct k_spinlock *p, k_spinlock_key_t key)
{
    p->held = 0;
    enabled = key;
}

void initialize(void)
{
    a = c = d = k = l = r = bad = 0;
    enabled = 1;
    driver.lock.held = 0;
}

void device_irq(void)
{
    c++;
}

static int rzt2m_module_start(const struct device *dev)
{
    if (!dev || !dev->config || !dev->data) {
        return -ENODEV;
    }

    const struct rzt2m_device_config *config = dev->config;
    struct rzt2m_device_data *data = dev->data;
    int interface_id = BASE_TO_IFACE_ID(config->base);
    unsigned int irqkey;
    STEP(0, {
        irqkey = irq_lock();
        k = (int)irqkey;
    });
    volatile uint32_t dummy;

    k_spinlock_key_t key;
    STEP(1, {
        key = k_spin_lock(&data->lock);
        l = (int)key;
    });

    if (interface_id < 5) {
        /* Dummy-read at least one time as stated in 8.3.1 of the User's Manual: Hardware */
        *MSTPCRA &= ~(MSTPCRA_MASK_SCIx(interface_id));
        dummy = *MSTPCRA;
    } else {
        LOG_ERR("SCI modules in the secure domain on RZT2M are not supported.");
        STEP(2, { k_spin_unlock(&data->lock, key); });
        STEP(3, { irq_unlock(irqkey); });
        return -ENOTSUP;
    }

    /* Dummy-read at least five times as stated in 8.3.1 of the User's Manual: Hardware */
    dummy = *RDR(config->base);
    dummy = *RDR(config->base);
    dummy = *RDR(config->base);
    dummy = *RDR(config->base);
    dummy = *RDR(config->base);

    k_spin_unlock(&data->lock, key);
    irq_unlock(irqkey);
    return 0;
}

void foreground(void)
{
    r = rzt2m_module_start(&dev);
    STEP(4, { bad = (a != 0 || b != 0 || enabled != 1); });
}
