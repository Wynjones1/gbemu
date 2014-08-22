all: build
	cd build; make

build:
	mkdir -p build
	cd build; cmake ..

run: all
	./bin/gbemu

clean:
	rm -Rf build bin *.svg *.out

prof:
	gprof ./bin/gbemu | gprof2dot.py | dot -Tsvg -o profile.svg
