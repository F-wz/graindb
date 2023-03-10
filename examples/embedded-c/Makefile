

.PHONY: graindb clean main

all: graindb main

clean:
	rm -rf build

graindb:
	cd ../.. && make

main:
	mkdir -p build
	cd build && cmake .. && make
	build/example


