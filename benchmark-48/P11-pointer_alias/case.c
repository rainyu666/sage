/* Interrupt behavior test case. */
int a, b, c, d, k, l, r, enabled, bad;

int *slot(void)
{
    return &a;
}

void initialize(void)
{
    a = b = c = d = k = l = r = bad = 0;
    enabled = 1;
    a = 2;
}

void device_irq(void)
{
    *slot() = 0;
}

void foreground(void)
{
    STEP(0, { c = *slot(); });
    STEP(1, {
        if (c > 0) {
            d = c;
            bad = (d == 0);
            if (!bad)
                r = 18 / d;
        }
    });
}
