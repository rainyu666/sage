/* cache.c - d-cache support for ARC CPUs */

#include <stdint.h>
#include <stddef.h>
int a, b, c, d, k, l, r, enabled, bad;
static const unsigned sys_dcache_line_size = 64;
#define ROUND_DOWN(v, n) ((v) & ~((uintptr_t)(n) - 1))
#define _ARC_V2_DC_CTRL 1
#define _ARC_V2_DC_IVDL 2
#define DC_CTRL_INVALIDATE_MODE 4
#define __builtin_arc_nop() ((void)0)

uint32_t z_arc_v2_aux_reg_read(unsigned reg)
{
    (void)reg;
    return 0;
}

void z_arc_v2_aux_reg_write(unsigned reg, uintptr_t val)
{
    (void)reg;
    (void)val;
}

unsigned arch_irq_lock(void)
{
    unsigned key = enabled;
    enabled = 0;
    return key;
}

void arch_irq_unlock(unsigned key)
{
    enabled = key;
}

void irq_unlock(unsigned key)
{
    a--;
    enabled = key;
}

void initialize(void)
{
    a = 1;
    b = c = d = k = l = r = bad = 0;
    enabled = 0;
}

void device_irq(void)
{
    c++;
}

static void dcache_invalidate_lines(void *start_addr_ptr, size_t size)
{
    uintptr_t start_addr = (uintptr_t)start_addr_ptr;
    uintptr_t end_addr;
    unsigned int key;
    uint32_t ctrl;

    end_addr = start_addr + size;
    start_addr = ROUND_DOWN(start_addr, sys_dcache_line_size);

    STEP(0, {
        key = arch_irq_lock();
        k = (int)key;
    }); /* -enter critical section- */

    ctrl = z_arc_v2_aux_reg_read(_ARC_V2_DC_CTRL);
    ctrl &= ~DC_CTRL_INVALIDATE_MODE;
    z_arc_v2_aux_reg_write(_ARC_V2_DC_CTRL, ctrl);

    do {
        z_arc_v2_aux_reg_write(_ARC_V2_DC_IVDL, start_addr);
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        start_addr += sys_dcache_line_size;
    } while (start_addr < end_addr);
    STEP(1, { arch_irq_unlock(key); }); /* -exit critical section- */
}

void foreground(void)
{
    dcache_invalidate_lines((void *)0x1000, 64);
    STEP(2, { bad = (a != 1 || enabled != 0); });
}
