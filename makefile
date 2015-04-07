BUILD_CONFIG ?= Debug
BUILD_CONFIG ?= Release
BUILD_CONFIG ?= RelWithDebInfo
ROM_DIR      ?= ~/roms/
ROM          ?= "dmg_sound/rom_singles/01-registers.gb"
ROM          ?= mario.gb
ROM          ?= "cpu_instrs/cpu_instrs.gb"

COVERAGE     ?= No
SPINLOCK     ?= No
MINGW        ?= No
EXE          := gbemu
CHECK        ?= No
CC           := clang
TESTING      ?= No
AUDIO        ?= Yes
DEBUG_WINDOW ?= Yes


all: build
	cd build && make

build/Makefile:
	mkdir -p build
	cd build; CC=$(CC) cmake -DCOVERAGE=$(COVERAGE) -DMINGW=$(MINGW) \
		-DSPINLOCK=$(SPINLOCK) -DCMAKE_BUILD_TYPE=$(BUILD_CONFIG) \
		-DAUDIO=$(AUDIO) -DCHECK=$(CHECK) -DTESTING=$(TESTING)    \
		-DDEBUG_WINDOW=$(DEBUG_WINDOW) ..

build: build/Makefile
	cd build; make -j5

mingw: MINGW := Yes EXE
mingw: all

run: all
	./build/$(EXE) -i $(ROM_DIR)/$(ROM) --break-file break.txt

gdb: all
	cgdb --args ./build/$(EXE) -i $(ROM_DIR)/$(ROM)

clean:
	rm -Rf build bin *.svg *.out *.ppm *.log coverage

android:
	cd android-project/jni/src/ && ndk-build -j5
	cd android-project/ && ant release && ant debug install

windows:
	make -f mingw.mk

coverage: COVERAGE := Yes
coverage: clean run
	lcov --directory ./build/ --capture --output-file ./build/coverage.info
	lcov --remove ./build/coverage.info "/usr/*" --output-file ./build/coverage.temp
	mv ./build/coverage.temp ./build/coverage.info
	genhtml -o ./build/coverage ./build/coverage.info
