/* Interrupt behavior test case. */
int a, b, c, d, k, l, r, enabled, bad;

int count(void)
{
    return (a - b) & 7;
}

void initialize(void)
{
    a = b = c = d = k = l = r = bad = 0;
    enabled = 1;
    a = 1;
    b = 0;
}

void device_irq(void)
{
    b = a;
}

void foreground(void)
{
    STEP(0, { c = count(); });
    STEP(1, {
        if (c > 0) {
            d = count();
            bad = (d == 0);
            if (!bad)
                r = 12 / d;
        }
    });
}
