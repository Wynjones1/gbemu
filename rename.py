#!/usr/bin/env python
import os
import subprocess

for root, dirs, files in os.walk("./src/"):
	for file in files:
		file = os.path.join(root, file)
		if file.endswith(".c"):
			command = "mv %s %spp" % (file, file)
			subprocess.call(command, shell=True)
