#!/usr/bin/env python2.7
import sys

if __name__ == "__main__":
    fp = open(sys.argv[1], "r") 
    for line in iter(fp.readline, ""):
        print(line.split(",")[5])
