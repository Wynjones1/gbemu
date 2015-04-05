BUILD_CONFIG ?= RelWithDebInfo
BUILD_CONFIG ?= Debug
ROM_DIR      ?= ~/roms/
ROM          ?= mario.gb

#Succeeding
ROM          := "cpu_instrs/individual/06-ld r,r.gb"
ROM          := "cpu_instrs/individual/09-op r,r.gb"
ROM          := "cpu_instrs/individual/05-op rp.gb"
ROM          := "cpu_instrs/individual/10-bit ops.gb"
ROM          := "cpu_instrs/individual/04-op r,imm.gb"
ROM          := "cpu_instrs/individual/02-interrupts.gb"
ROM          := "cpu_instrs/individual/01-special.gb"
ROM          := "cpu_instrs/individual/11-op a,(hl).gb"

#Failing
ROM          := "cpu_instrs/individual/07-jr,jp,call,ret,rst.gb"
ROM          := "cpu_instrs/individual/08-misc instrs.gb"
ROM          := "cpu_instrs/individual/03-op sp,hl.gb"



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
	./build/$(EXE) -i $(ROM_DIR)/$(ROM) --break-file ./break.txt

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
