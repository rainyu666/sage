/* Interrupt behavior test case. */
int a, b, c, d, k, l, r, enabled, bad;

int gate(int use)
{
    int old = enabled;
    if (use)
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
    STEP(0, { k = gate(0); });
    STEP(1, { c = a; });
    STEP(2, { d = b; });
    STEP(3, { ungate(k); });
    STEP(4, { bad = (c != d); });
}
