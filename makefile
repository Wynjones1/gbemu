all: build
	#cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..; make -j5
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make -j5
#	cd build; cmake  ..; make -j5

build:
	mkdir -p build

run: all
	./build/gbemu -i ~/roms/mario.gb

clean:
	rm -Rf build bin *.svg *.out *.ppm *.log

prof:
	gprof ./bin/gbemu | gprof2dot.py | dot -Tsvg -o profile.svg
