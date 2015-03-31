#!/usr/bin/env python2.7
from collections import defaultdict
import re

if __name__ == "__main__":
    ifp = open("instr.txt", "r")
    ofp = open("out.txt", "w")
    d = defaultdict(lambda : defaultdict(lambda : None))
    call_regex = re.compile(r"CALL (.{2},)?0x([0-9A-Za-z]{4})")
    function_map = dict()
    for i in ifp:
        bank, instr, val = i.strip().split(":")
        match = call_regex.match(val)
        if match:
            addr = int(match.group(2), 16)
            if addr not in function_map:
                function_map[addr] = True
        d[int(bank, 16)][int(instr, 16)] = val
    count = 0
    for i, j in d.items():
        for addr in xrange(0x10000):
            if j[addr]:
                if addr in function_map:
                    ofp.write("{:04X}:{:04X}:{}             function {}\n".format(i, addr, j[addr],count))
                    count += 1
                else:
                    ofp.write("{:04X}:{:04X}:{}\n".format(i, addr, j[addr]))
