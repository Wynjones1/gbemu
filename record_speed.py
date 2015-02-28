#!/usr/bin/env python
import sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
    if len(sys.argv) == 1:
        data = []
        for line in sys.stdin:
            if("INFO: CLKSPEED" in line):
                speed = line[-8:-2]
                data.append(float(speed))
        plt.plot(data)
    else:
        for i in sys.argv[1:]:
            data = []
            for line in open(i, "r"):
                if("INFO: CLKSPEED" in line):
                    speed = line[-8:-2]
                    data.append(float(speed))
            plt.plot(data)
    plt.show()
