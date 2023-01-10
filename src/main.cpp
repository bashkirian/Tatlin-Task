#include <string>
#include <stdio.h> 
#include <stdlib.h>   
#include <iostream>
#include <fstream>
#include <thread> 

using namespace std;

#include "Commander.h"
#include "Utils.h"
#include "Tape.h"

int main(int argc, char ** argv) {
	
	if (argc <= 2) {
		cout << "Need more params: first param is input file, second param is output file";
		return 0;
	}
	
	int mem_size = 100; // допустим, 100МБ
	int max_thread_count = std::thread::hardware_concurrency(); // максимальное количество потоков
	string unsorted_file = string(argv[1]);
	string sorted_file = string(argv[2]);

	ifstream tape_params("./params.txt");
	if (!tape_params.is_open()) {
		cout << "Could not open params.txt file" << endl;
		return -1;
	}
	string param_name;
	int read, write, rewind, move;
	while (tape_params >> param_name) {
		if (param_name == "read")
			tape_params >> read;
		else 
		{
			if (param_name == "write")
				tape_params >> write;
			else {
				if (param_name == "rewind")
					tape_params >> rewind;
				else {
					if (param_name == "move")
						tape_params >> move;
				}
			}
		}
	}

	tp.move_delay = move;
	tp.read_delay = read;
	tp.rewind_delay = rewind;
	tp.write_delay = write;
	
	if (mem_size == 0) {
		cout << "Memory size can't be zero";
		return 0;
	}
	
	if (max_thread_count == 0) {
		cout << "Max thread count can't be zero";
		return 0;
	}

	try {
		
		Commander c(mem_size, 1, "./dir/" + unsorted_file, "./dir/" + sorted_file);
		c.sort();
		cout << "Successfully saved to file "<< sorted_file << endl;
	} catch (std::exception & exc) {
		cout << exc.what() << "-----------" << endl;
	}
	
	return 0;
}

