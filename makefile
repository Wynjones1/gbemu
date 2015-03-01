BUILD_CONFIG ?= RelWithDebInfo
BUILD_CONFIG ?= Debug
ROM_DIR      ?= ~/roms/
ROM          ?= mario.gb
ROM          ?= pokemon_blue.gb
COVERAGE     ?= No
SPINLOCK     ?= No
MINGW        ?= No
EXE          := gbemu
CHECK        ?= No
CC           := gcc


all: build
	cd build && make

build/Makefile:
	mkdir -p build
	cd build; CC=$(CC) cmake -DCOVERAGE=$(COVERAGE) -DMINGW=$(MINGW) \
		-DSPINLOCK=$(SPINLOCK) -DCMAKE_BUILD_TYPE=$(BUILD_CONFIG) -DCHECK=$(CHECK) ..

build: build/Makefile
	cd build; make -j5

mingw: MINGW := Yes EXE
mingw: all

run: all
	./build/$(EXE) -i $(ROM_DIR)/$(ROM)

clean:
	rm -Rf build bin *.svg *.out *.ppm *.log coverage

android:
	cd android-project/jni/src/ && ndk-build -j5
	cd android-project/ && ant release && ant debug install
