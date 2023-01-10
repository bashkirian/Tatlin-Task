CC=g++
CFLAGS=-Wall -std=c++0x 

all:
	rm -rf ./dir;
	mkdir ./dir;
	make sort;
	make checker;
	make generator;
sort:
	$(CC) $(CFLAGS) -lpthread  src/main.cpp src/Utils.cpp src/Commander.cpp src/SortThread.cpp -o sort.out
checker:
	$(CC) $(CFLAGS) src/BinaryChecker.cpp src/Utils.cpp  -o checker.out
generator: 
	$(CC) $(CFLAGS) src/BinaryGenerator.cpp src/Utils.cpp  -o generator.out
generate_tapes:
	rm -rf ./dir;
	mkdir ./dir;
	./generator.out 10 10;
	./generator.out 128 128
	./generator.out 10000 10000;
run_tests:
	make generate_tapes; 
	 ./sort.out 10 10_out;
	 ./checker.out 10_out; 
	 ./sort.out 128 128_out;
	 ./checker.out 128_out; 
	 ./sort.out 10000 10000_out;
	 ./checker.out 10000_out; 
clean: 
	rm -rf *.out;
	rm -rf ./dir;
