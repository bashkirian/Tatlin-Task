#include <string>
#include <stdio.h> 
#include <stdlib.h>   
#include <iostream>
#include <fstream>

using namespace std;

#include "Utils.h"

int main(int argc, char ** argv) {
	
	if (argc <= 1) {
		cout << "Pls enter sorted file name to check" << endl;
		return 0;
	}
	
	string sorted_file = string(argv[1]);
	
	if (sorted_file == "") {
		cout << "Error: wrong unsorted file name";
		return 0;
	}

	try {
		
		Utils f = Utils();
		f.checkSortedFile(sorted_file);

	} catch (std::exception & exc) {
		cout << exc.what() << endl;
	}
	
	return 0;
}

