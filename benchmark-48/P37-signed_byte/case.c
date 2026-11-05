/* Interrupt behavior test case. */
#include <stdint.h>
int a, b, c, d, k, l, r, enabled, bad;

void initialize(void)
{
    a = b = c = d = k = l = r = bad = 0;
    enabled = 1;
    a = 127;
}

void device_irq(void)
{
    a = 128;
}

void foreground(void)
{
    STEP(0, { c = a; });
    STEP(1, {
        if (c < 128) {
            d = (int8_t)a;
            bad = (d < 0);
        }
    });
}
