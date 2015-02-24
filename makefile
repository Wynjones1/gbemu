BUILD_CONFIG ?= Debug
ROM_DIR      ?= ~/roms/
ROM          ?= pokemon_blue.gb
ROM          ?= mario.gb
COVERAGE     ?= No
SPINLOCK     ?= No
MINGW        ?= No
EXE          := gbemu
CHECK        ?= No


all: build
	cd build && make

build:
	mkdir -p build
	cd build; cmake -DCOVERAGE=$(COVERAGE) -DMINGW=$(MINGW) -DSPINLOCK=$(SPINLOCK) -DCMAKE_BUILD_TYPE=$(BUILD_CONFIG) -DCHECK=$(CHECK) ..

mingw: MINGW := Yes EXE := gbemu.exe
mingw: all

run: all
	./build/$(EXE) -i $(ROM_DIR)/$(ROM)

clean:
	rm -Rf build bin *.svg *.out *.ppm *.log coverage

prof:
	gprof ./bin/gbemu | gprof2dot.py | dot -Tsvg -o profile.svg

plot: run
	cat ./track.txt | ./plot.py

coverage: BUILD_CONFIG=Debug
coverage: COVERAGE=Yes
coverage: clean all
	GCOV_PREFIX_STRIP=100 GCOV_PREFIX=$(shell pwd)/coverage ./build/$(EXE) -i $(ROM_DIR)/$(ROM)
	find ./build -type f -name *.gcno -exec ln -s $(shell pwd)/{} ./coverage \;
	cd coverage && lcov --directory ./ --capture --output-file coverage.info
	cd coverage && lcov --remove coverage.info "src/ppm.c" "src/debug.c" "src/instructions/invalid.c" '/usr/*' --output-file coverage.info.clean
	cd coverage && genhtml -o out.html coverage.info.clean
	firefox ./coverage/out.html/index.html

gen_report:
	cd coverage && lcov --directory ./ --capture --output-file coverage.info
	cd coverage && lcov --remove coverage.info '/usr/*' --output-file coverage.info.clean
	cd coverage && genhtml -o out.html coverage.info.clean
	firefox ./coverage/out.html/index.html

check: CHECK=Yes
check: clean build
	make -k
