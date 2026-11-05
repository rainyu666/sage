/* Interrupt behavior test case. */
#include <stdint.h>
int a, b, c, d, k, l, r, enabled, bad;

void initialize(void)
{
    a = b = c = d = k = l = r = bad = 0;
    enabled = 1;
    a = 65535;
}

void device_irq(void)
{
    a = 65536;
}

void foreground(void)
{
    STEP(0, { c = a; });
    STEP(1, {
        if (c == 65535) {
            d = (uint16_t)c;
            bad = (d == 0);
        }
    });
}
