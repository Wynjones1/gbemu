#!/usr/bin/env python2.7
import subprocess

out_dir = "profile"
subprocess.call("mkdir -p %s" % out_dir, shell = True);
for i in xrange(3):
    p = subprocess.Popen("./build/gbemu --replay", shell =True, stderr = open("./%s/%d.txt" % (out_dir, i), "w"))
    p.wait()
