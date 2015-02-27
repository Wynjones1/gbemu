#!/bin/bash
ndk-build clean && ndk-build NDK_DEBUG=0 V=1 -j5
cd ~/C/gbemu/android-project
ant debug install
