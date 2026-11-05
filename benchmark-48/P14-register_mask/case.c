/* Interrupt behavior test case. */
int a, b, c, d, k, l, r, enabled, bad;

int status(void)
{
    return enabled << 3;
}

void write_status(int v)
{
    enabled = (v & 8) != 0;
}

void initialize(void)
{
    a = b = c = d = k = l = r = bad = 0;
    enabled = 1;
    a = 1;
    b = 1;
}

void device_irq(void)
{
    a = 2;
    b = 2;
}

void foreground(void)
{
    STEP(0, { k = status(); });
    STEP(1, { write_status(k & ~8); });
    STEP(2, { c = a; });
    STEP(3, { d = b; });
    STEP(4, { write_status(k); });
    STEP(5, { bad = (c != d); });
}
