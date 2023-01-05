#pragma once 

#include <mutex>
#include <vector>
#include <sys/types.h>

using namespace std;

#define MB 1024 * 1024

class Commander {

private:
	uint c_thread_count; //actual thread count
	uint c_mem_mb_size; //memory size in bytes
	
	string c_unsorted_file; //unsorted file name
	string c_sorted_file; //sorted file name
	
	mutex * c_locker; //mutex to lock unlock sorted file write 
	
	uint c_file_size; //sorted and unsorted files size's in bytes
public: 
 	

 	Commander (const uint mem_size, const uint max_thread_count, const string unsorted_file, const string sorted_file);
 	
 	void sort ();
 	/*
 	*
 	* @desc multithreading sort blocks function
 	*
 	*/
 	void sortBlocks ();
 	
 	/*
 	*
 	* @desc Merge sorted blocks function
 	*
 	*/
 	void mergeSortedBlocks (); 

 	/*
 	*
 	* @desc Read bytes to buffer with size bytes, from bpos position in vector, from the io stream and from position pos
 	* @buffer - vector where write's the int32_t numbers from stream
 	* @bpos - position in buffer from start to write numbers
 	* @bytes - count of bytes to read from IO and write to buffer
 	*/
 	static void readBinaryFile(vector<int32_t> * buffer, const uint bpos, const uint pos, const uint bytes, ifstream & io);

 	/*
 	*
 	* @desc Write size * size(int32) bytes to io from bpos in buffer
 	* @buffer - vector where read the int32_t numbers
 	* @bpos - position in buffer from write's to IO
 	* @size - count of numbers write to IO
 	* @pos - seek in IO
 	*/
 	static void writeBinaryFile(vector<int32_t> * buffer, const uint bpos, const uint size, const uint pos, ofstream & io);

 	~Commander();
};
