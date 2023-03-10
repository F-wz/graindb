.PHONY: graindb clean main

all: graindb main

clean:
	rm *.class

graindb:
	make -C ../.. jdbc

main:
	javac GrainDBExample.java
	java -cp .:../../build/release/tools/jdbc/graindb_jdbc.jar GrainDBExample

