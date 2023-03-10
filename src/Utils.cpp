#include "Utils.h"

#include <ctime>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>


void Utils::generateBinaryFile (const std::string file_name, const uint sign_number) {
	ofstream out("./dir/" + file_name, ios::binary | ios::out | ios::in | ios::trunc);
	uint buff_size = MAX_BUFFER_SIZE > sign_number ? sign_number : MAX_BUFFER_SIZE;

    std::vector<int32_t> buff(buff_size);
    srand(time(0));
    uint seek = 0;

	while (seek < sign_number * sizeof(int32_t) ) {

    	if (seek / sizeof(int32_t) + buff_size > sign_number) {
    		buff_size = sign_number - seek / sizeof(int32_t);
    	}

    	out.seekp(seek, ios::beg);
    	for (uint i = 0; i < buff_size; i++) {
    		buff[i] =  (i % 2 == 0 ? -1 : 1) * (rand() % 25 + 1);
	    }

	    out.write(reinterpret_cast<const char*>(&buff[0]), buff_size * sizeof(int32_t));
	    seek += buff_size * sizeof(int32_t);
    } 

	out.close();
}


void Utils::checkSortedFile (std::string file_name) {

	uint buff_size = MAX_BUFFER_SIZE;
	
	ifstream in("./dir/" + file_name, ios::binary | ios::ate);
	std::vector<int32_t> buffer(buff_size);

	uint pos = 0;
	bool is_valid = true;
	uint read_bytes = 0;
	int value;
	do  {
		in.seekg(pos, ios::beg);
		in.read(reinterpret_cast<char*>( & buffer[0] ),  buff_size * sizeof(int32_t));
		read_bytes = in.gcount();
		
		if (pos == 0) {
			value = buffer[0];
		}

		for (uint i = 0; i < (read_bytes / sizeof(int32_t)); i++) {
			if (is_valid && buffer[i] < value) {
				is_valid = false;
	   		}
	   		value = buffer[i];
    	}

    	pos += buff_size * sizeof(int32_t);
	} while(read_bytes == buff_size * sizeof(int32_t));

	if (!is_valid) {
		cout << "Tape " << file_name << " is not sorted" << endl;
	} else {
		cout << "Tape " << file_name <<  " is sorted" << endl;
	}
}

void Utils::mergeSortTapes(Tape* src, Tape* buffer, uint spos, uint size) {
    uint i = 0, j = 0, k = 0;
    while (i < spos && j < size - spos)  
	    buffer->write(k++, (*src)[i] < (*src)[j + spos] ? (*src)[i++] :  (*src)[j++ + spos]);
    while (i < spos)  
		buffer->write(k++, (*src)[i++]);
    while (j < size - spos)    
		buffer->write(k++, (*src)[j++ + spos]);
}

int compare (const void * one, const void * two)
{
	return *((int*)one) - *((int*)two);   
}

