#include <iostream>
#include <fstream>
#include <vector>

#include "SortThread.h"
#include "Utils.h"

SortThread::SortThread (const std::string out_file, const std::string in_file, const uint number_count, const uint seek, mutex * locker) : 
			st_seek(seek),
			st_out_file(out_file), 
			st_in_file(in_file),
			st_number_count(number_count),
			st_locker(locker)
			
{
	st_mem_size = number_count * sizeof(int32_t);
}

void * SortThread::run(void * obj) {
	((SortThread*)obj)->readBlockSortAndWrite();
	delete ((SortThread*)obj);
	return NULL;
}

void SortThread::readBlockSortAndWrite() {
	
	ifstream in(st_in_file, ios::binary | ios::in);
	
	std::vector<int32_t> buff(st_number_count);
	
	in.seekg(st_seek);		
	in.read(reinterpret_cast<char*>(buff.data()), st_mem_size);
	in.close();

	qsort(&buff[0], buff.size(), sizeof(int32_t), compare);
	
	st_locker->lock();
	
	ofstream out(st_out_file, ios::binary | ios::out | ios::in);
	out.seekp(st_seek);
    out.write(reinterpret_cast<const char*>(&buff[0]), st_mem_size);
    out.close();

    st_locker->unlock();
}