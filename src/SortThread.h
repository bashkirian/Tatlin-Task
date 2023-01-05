#pragma once

#include <mutex>

using namespace std;

class SortThread {
private :
	uint st_mem_size;//block size of thread
	uint st_seek;// position of the memory where we write block
	
	string st_out_file;
	string st_in_file;

	uint st_number_count;
	mutex * st_locker;
public :

	SortThread (const string out_file, const string in_file, const uint number_count, const uint seek, mutex * locker);

	static void * run (void * obj);
	void readBlockSortAndWrite();
	void print();
};
