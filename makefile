BUILD_CONFIG ?= Debug
BUILD_CONFIG ?= Release
ROM_DIR      ?= ~/roms/
ROM          ?= "cpu_instrs/individual/01-special.gb"
ROM          ?= "cpu_instrs/individual/09-op r,r.gb"
ROM          ?= "cpu_instrs/individual/04-op r,imm.gb"
ROM          ?= "cpu_instrs/individual/07-jr,jp,call,ret,rst.gb"
ROM          ?= "cpu_instrs/individual/06-ld r,r.gb"
ROM          ?= cpu_instrs/cpu_instrs.gb
ROM          ?= mario.gb
COVERAGE     ?= No
SPINLOCK     ?= No
MINGW        ?= No
EXE          := gbemu
CHECK        ?= No
CC           := gcc
TESTING      ?= No
AUDIO        ?= Yes


all: build
	cd build && make

build/Makefile:
	mkdir -p build
	cd build; CC=$(CC) cmake -DCOVERAGE=$(COVERAGE) -DMINGW=$(MINGW) \
		-DSPINLOCK=$(SPINLOCK) -DCMAKE_BUILD_TYPE=$(BUILD_CONFIG) \
		-DAUDIO=$(AUDIO) -DCHECK=$(CHECK) -DTESTING=$(TESTING) ..

build: build/Makefile
	cd build; make -j5

mingw: MINGW := Yes EXE
mingw: all

run: all
	./build/$(EXE) -i $(ROM_DIR)/$(ROM) --break-file ./break.txt

clean:
	rm -Rf build bin *.svg *.out *.ppm *.log coverage

android:
	cd android-project/jni/src/ && ndk-build -j5
	cd android-project/ && ant release && ant debug install

windows:
	make -f mingw.mk
