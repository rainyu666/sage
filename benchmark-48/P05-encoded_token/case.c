/* Interrupt behavior test case. */
int a, b, c, d, k, l, r, enabled, bad;

int gate(void)
{
    int old = enabled << 2;
    enabled = 0;
    return old;
}

void ungate(int key)
{
    enabled = (key & 4) != 0;
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
    STEP(0, { k = gate(); });
    STEP(1, { ungate(0); });
    STEP(2, { c = a; });
    STEP(3, { d = b; });
    STEP(4, { ungate(k); });
    STEP(5, { bad = (c != d); });
}
