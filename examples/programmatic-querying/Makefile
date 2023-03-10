

.PHONY: graindb clean main

all: graindb main

clean:
	rm -rf build

graindb:
	make  -C ../../

main:
	mkdir -p build
	cd build && cmake .. && make
	build/programmating_querying_example


