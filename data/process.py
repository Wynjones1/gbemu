#!/usr/bin/env python2.7

CLK_SPEED = 4194304
if __name__ == "__main__":
    fp = open("clockspeed.txt", "r")
    for i in fp:
        try:
            time, clks = map(int, i.strip().split())
            time = time / 1000.0
            print(clks / (CLK_SPEED * time))
        except ValueError: pass
