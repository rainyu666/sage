/* Interrupt behavior test case. */
#include <stdint.h>
int a, b, c, d, k, l, r, enabled, bad;

uint8_t sample(void)
{
    return a;
}

void initialize(void)
{
    a = b = c = d = k = l = r = bad = 0;
    enabled = 1;
    a = 255;
}

void device_irq(void)
{
    a = 256;
}

void foreground(void)
{
    STEP(0, { c = sample(); });
    STEP(1, {
        if (c == 255) {
            d = sample();
            bad = (d == 0);
        }
    });
}
