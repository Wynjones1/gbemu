#!/usr/bin/env python
from gen_noise_table import *

CLK = 4194304
def gen_freqs():
    s = set()
    for shift in xrange(15):
        for div in xrange(8):
            x = 8 * div
            if div == 0:
                x = 4
            freq = (CLK / x) >> (shift)
            s.add(freq)
    return list(s)

def gen_clk():
    s = set()
    for shift in xrange(1,15):
        for div in xrange(8):
            x = 8 * div
            if div == 0:
                x = 4
            freq = (CLK / x) >> (shift)
            s.add(freq)
    return list(s)

if __name__ == "__main__":
    """
    table_7 = gen_table(7)
    table_15 = gen_table(15)

    l7 = len(table_7)
    l15 = len(table_15)
    print(l7, l15)
    gen_freqs()
    """
    for x in sorted(gen_clk()):
        print("{} {}".format(x, CLK / x))