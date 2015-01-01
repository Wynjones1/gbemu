all: build
	cd build; make -j4

build:
	mkdir -p build
	cd build; cmake ..

run: all
	./build/gbemu

clean:
	rm -Rf build bin *.svg *.out *.ppm *.log

prof:
	gprof ./bin/gbemu | gprof2dot.py | dot -Tsvg -o profile.svg
