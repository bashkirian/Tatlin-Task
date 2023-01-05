#include <string>
#include <stdio.h> 
#include <stdlib.h>   
#include <iostream>
#include <fstream>

using namespace std;

#include "Utils.h"

int main(int argc, char ** argv) {
	
	if (argc <= 2) {
		cout << "Need more params: first param is generated unsorted filename, second param is number count" << endl;
		return 0;
	}
	
	string unsorted_file = string(argv[1]);
	uint number_count = atoi(argv[2]);

	if (number_count <= 0) {
		cout << "Error: Wrong number count";
		return 0;
	}
	
	if (unsorted_file == "") {
		cout << "Error: wrong unsorted file name";
		return 0;
	}

	try {
		Utils f = Utils();
		f.generateBinaryFile(unsorted_file, number_count);
		cout << "Succefully saved to file " << unsorted_file << endl;
	} catch (std::exception & exc) {
		cout << exc.what() << endl;
	}
	
	return 0;
}

