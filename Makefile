CC=g++
CFLAGS=-Wall -std=c++0x 

sort:
	$(CC) $(CFLAGS) -lpthread  src/main.cpp src/Utils.cpp src/Commander.cpp src/SortThread.cpp -o sort.out
checker:
	$(CC) $(CFLAGS) src/BinaryChecker.cpp src/Utils.cpp  -o checker.out
generator: 
	$(CC) $(CFLAGS) src/BinaryGenerator.cpp src/Utils.cpp  -o generator.out
clean: 
	rm -rf *.out
