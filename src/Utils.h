#pragma once

#include <string>
#include <sys/types.h>
#include <vector>
#include "Tape.h"
using namespace std;

#define MAX_BUFFER_SIZE 64 * 1024 * 1024

class Utils {
public :
	Utils () {}
	void generateBinaryFile (const std::string file_name, const uint sign_number);
	void checkSortedFile (std::string file_name);
	static void mergeSortVectors(vector<int32_t> * src, vector<int32_t> * buffer, uint spos, uint size);
	static void mergeSortTapes(Tape* src, Tape* buffer, uint spos, uint size);
	static void printVectorStat(vector<int32_t> * vec);
};

int compare (const void * one, const void * two);