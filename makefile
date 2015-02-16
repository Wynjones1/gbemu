BUILD_CONFIG ?= Release
ROM_DIR      ?= ~/roms/
ROM          ?= mario.gb
SPINLOCK     ?= No
MINGW        ?= No
EXE          := gbemu

all: build
	cd build; cmake -DMINGW=$(MINGW) -DSPINLOCK=$(SPINLOCK) -DCMAKE_BUILD_TYPE=$(BUILD_CONFIG) ..; make -j5

build:
	mkdir -p build

mingw: MINGW := Yes EXE := gbemu.exe
mingw: all

run: all
	./build/$(EXE) -i $(ROM_DIR)/$(ROM)

clean:
	rm -Rf build bin *.svg *.out *.ppm *.log

prof:
	gprof ./bin/gbemu | gprof2dot.py | dot -Tsvg -o profile.svg

plot: run
	cat ./track.txt | ./plot.py
