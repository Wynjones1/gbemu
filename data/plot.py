#!/usr/bin/env python2.7
import matplotlib.pyplot as plt
import sys

x = []
y = []
for i in sys.stdin:
    t = i.strip().split(" ")
    if len(t) == 2:
        x.append(t[0])
        y.append(t[1])
plt.scatter(x, y)
plt.show()
