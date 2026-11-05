/* Interrupt behavior test case. */
int a, b, c, d, k, l, r, enabled, bad;

void initialize(void)
{
    a = b = c = d = k = l = r = bad = 0;
    enabled = 1;
    a = 255;
    b = 0;
}

void device_irq(void)
{
    a = 0;
    b = 1;
}

void foreground(void)
{
    STEP(0, {
        k = enabled;
        enabled = 0;
    });
    STEP(1, { c = a; });
    STEP(2, { d = b; });
    STEP(3, { enabled = k; });
    STEP(4, {
        r = c + 256 * d;
        bad = (r != 255 && r != 256);
    });
}
