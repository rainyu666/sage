/* Interrupt behavior test case. */
int a, b, c, d, k, l, r, enabled, bad;

int gate(void)
{
    int old = enabled;
    enabled = 0;
    return old;
}

void ungate(int key)
{
    enabled = key;
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
    STEP(1, { l = gate(); });
    STEP(2, { ungate(1); });
    STEP(3, { c = a; });
    STEP(4, { d = b; });
    STEP(5, { ungate(k); });
    STEP(6, { bad = (c != d); });
}
