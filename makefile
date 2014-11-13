all:
	mkdir -p build
	cd build; cmake .. -DCMAKE_BUILD_TYPE=Debug; make -j5

run: all
	./build/gbemu

clean:
	rm -Rf build bin *.svg *.out *.ppm *.log

prof:
	gprof ./bin/gbemu | gprof2dot.py | dot -Tsvg -o profile.svg
