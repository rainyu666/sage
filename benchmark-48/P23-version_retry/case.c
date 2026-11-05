/* Interrupt behavior test case. */
int a, b, c, d, k, l, r, enabled, bad;

void initialize(void)
{
    a = b = c = d = k = l = r = bad = 0;
    enabled = 1;
    a = 1;
    b = 1;
    r = 0;
}

void device_irq(void)
{
    r = r + 1;
    a = 2;
    b = 2;
    r = r + 1;
}

void foreground(void)
{
    STEP(0, { k = r; });
    STEP(1, { c = a; });
    STEP(2, { d = b; });
    STEP(3, { bad = (c != d); });
}
