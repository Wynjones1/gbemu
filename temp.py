#!/usr/bin/env python2.7
import os
from os.path import join as pjoin
import subprocess

for root, dirs, files in os.walk("src"):
    for file in files:
        if file.endswith(".cpp"):
            path = pjoin(root, file)
            command = "mv {} {}".format(path, path[:-2])
            subprocess.call(command, shell=True)
