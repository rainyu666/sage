/* Interrupt behavior test case. */
#include <stdint.h>
int a, b, c, d, k, l, r, enabled, bad;

void initialize(void)
{
    a = b = c = d = k = l = r = bad = 0;
    enabled = 1;
    a = 16;
}

void device_irq(void)
{
    a = 32;
}

void foreground(void)
{
    STEP(0, { c = a; });
    STEP(1, {
        if (c == 16) {
            d = (uint8_t)(c * 8);
            bad = (d == 0);
        }
    });
}
