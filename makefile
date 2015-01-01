all: build
	cd build; cmake ..; make -j5

build:
	mkdir -p build

run: all
	./build/gbemu

clean:
	rm -Rf build bin *.svg *.out *.ppm *.log

prof:
	gprof ./bin/gbemu | gprof2dot.py | dot -Tsvg -o profile.svg
