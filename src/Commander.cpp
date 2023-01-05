#include <pthread.h>
#include <iostream>
#include <fstream>

#include "Commander.h"
#include "SortThread.h"
#include "Utils.h"
#include "Tape.h"
Commander::Commander(const uint mem_size, const uint max_thread_count, const string unsorted_file, const string sorted_file) : 
				c_thread_count(max_thread_count),
				c_mem_mb_size(mem_size * MB),
				c_unsorted_file(unsorted_file),
				c_sorted_file(sorted_file)
				
{
	ifstream in(c_unsorted_file, ifstream::ate | ifstream::binary);
	c_file_size = in.tellg();
	in.close();

	c_mem_mb_size = c_mem_mb_size  > c_file_size ? c_file_size : c_mem_mb_size;

	while (c_mem_mb_size  % (c_thread_count * sizeof(int32_t)) != 0 && c_thread_count > 1) {
		c_thread_count--;
	}

	if (c_thread_count == 1) { // if we have only one thread, memory must divide on two cause in merge place sort 
		c_mem_mb_size = c_mem_mb_size / 2; 
	}
	
	c_locker = new mutex();
}

void Commander::sort () {
	sortBlocks();
	
	if (!(c_file_size <= c_mem_mb_size && c_thread_count == 1)) {
		mergeSortedBlocks();
	}
}

void Commander::sortBlocks () {
	ofstream ofs(c_sorted_file, std::ios::binary | std::ios::out);
	ofs.seekp(c_file_size - 1);
	ofs.write("", 1);
	ofs.close();

	int pthread_status;	

	uint each_thread_mbytes = c_mem_mb_size / c_thread_count;
		
	for (uint seek = 0; seek  < c_file_size; seek += c_mem_mb_size) {

		uint seek_in_bytes = 0; 	
		pthread_t * threads = new pthread_t[c_thread_count];

		uint actual_thread_count = 0;
		for (uint i = 0; i < c_thread_count && c_file_size > seek_in_bytes + seek; i++, actual_thread_count++) {

			uint number_count =  each_thread_mbytes / sizeof(int32_t);

			if (seek_in_bytes + seek + each_thread_mbytes  > c_file_size) {
				number_count = (c_file_size - (seek_in_bytes + seek)) / sizeof(int32_t);
			}
	    	
	    	SortThread * st = new SortThread(c_sorted_file, c_unsorted_file, number_count, seek + seek_in_bytes, c_locker);
	    	
	    	if ((pthread_status = pthread_create(&threads[i], NULL, &SortThread::run, (void*)st)) != 0) {
				throw runtime_error("Create thread exception with status " + to_string(static_cast<long long>(pthread_status)));
	    	}

	    	seek_in_bytes += each_thread_mbytes;
	    }

	    for (uint i = 0; i < actual_thread_count; i++) { 
	        if ((pthread_status = pthread_join(threads[i], NULL)) != 0) {
	        	throw runtime_error("Join thread error with status " + to_string(static_cast<long long>(pthread_status)));
	        }
	    }

	    delete[] threads;
	}
}

void Commander::readBinaryFile(vector<int32_t> * buffer, uint bpos, uint pos, uint bytes, ifstream & io) {
	io.seekg(pos, ios_base::beg);
	io.read(reinterpret_cast<char*>(  &(*buffer)[bpos] ), bytes);
}

void Commander::writeBinaryFile(vector<int32_t> * buffer, uint bpos, uint size, uint pos, ofstream & io) {
	io.seekp(pos, ios_base::beg);		
	io.write(reinterpret_cast<const char*>(&(*buffer)[bpos]), size * sizeof(int32_t));
	io.flush();
}

void Commander::mergeSortedBlocks () {
	uint merge_block_size = c_mem_mb_size  / c_thread_count;
	uint block_count = c_file_size / merge_block_size;
	
	ofstream out(c_sorted_file, ios::binary | ios::out | ios::in);
	ifstream in(c_sorted_file, ios::binary | ios::in);

	uint half_size = merge_block_size / sizeof(int32_t);
	
	// vector<int32_t> * block = new vector<int32_t>(half_size + half_size);
	// vector<int32_t> * buffer = new vector<int32_t>(half_size + half_size);

	Tape block_tape;
	Tape buffer_tape;
	block_tape.SetTapePointer(half_size + half_size);
	buffer_tape.SetTapePointer(half_size + half_size);
	vector<int32_t>* block = block_tape.GetTapePointer();
	vector<int32_t>* buffer = buffer_tape.GetTapePointer();

	for (uint gpos = 0; gpos < (uint)(block_count / 2); gpos++) {

		readBinaryFile(block, 0, gpos * merge_block_size, merge_block_size, in);
		
		for (uint lpos = gpos + 1; lpos < block_count; lpos++) {
			uint read_block_size = (lpos + 1) * merge_block_size > c_file_size ? c_file_size - (lpos) * merge_block_size : merge_block_size;
			readBinaryFile(block, half_size, lpos * merge_block_size, read_block_size, in);
			//Utils::mergeSortVectors(block, buffer, half_size, half_size + read_block_size / sizeof(int32_t));
			Utils::mergeSortTapes(&block_tape, &buffer_tape, half_size, half_size + read_block_size / sizeof(int32_t));
			vector<int> * v = block;
			block = buffer;
			buffer = v;
			writeBinaryFile(block, half_size, read_block_size / sizeof(int32_t), lpos * merge_block_size, out);
		}

		writeBinaryFile(block, 0, half_size, gpos * merge_block_size, out);
		
		uint read_block_size = (block_count - gpos) * merge_block_size > c_file_size ? c_file_size - (block_count - 1 - gpos) * merge_block_size : merge_block_size;
		readBinaryFile(block, half_size, (block_count - 1 - gpos) * merge_block_size, read_block_size, in);

		for (int lpos = block_count - 2 - gpos; lpos >= 0; lpos--) {
			readBinaryFile(block, 0, lpos * merge_block_size, merge_block_size, in);

		    //Utils::mergeSortVectors(block, buffer, read_block_size / sizeof(int32_t), half_size + read_block_size / sizeof(int32_t));
			Utils::mergeSortTapes(&block_tape, &buffer_tape, half_size, half_size + read_block_size / sizeof(int32_t));
			vector<int> * v = buffer;
			buffer = block;
			block = v;
			writeBinaryFile(buffer, 0, half_size, lpos * merge_block_size, out);
		}
		writeBinaryFile(buffer, half_size, read_block_size / sizeof(int32_t), (block_count - 1 - gpos) * merge_block_size, out);
		cout <<  (int)(100 * gpos / (block_count / 2)) << "%" << endl;
	}

	delete block;
	delete buffer;

	out.close();
	in.close();
}

Commander::~Commander() {
	delete c_locker;
}
