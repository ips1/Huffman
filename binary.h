#ifndef BINARY_H
#define BINARY_H

#include <fstream>
#include <stdint.h>

// Functions for printing longer numbers into binary files
// Independent on platform (little / big endian)
// Data from files have to be read by equivalent reading functions

void put_int64(std::ofstream& out, uint64_t number);

uint64_t get_int64(std::ifstream& in);

class BitStream
{
private:
	std::ofstream& out;
	uint8_t buffer;
	uint8_t state;

public:
	BitStream(std::ofstream& _out): out(_out), buffer(0), state(0) { }

	inline void print_bit(bool bit)
	{
		if (state == 8) 
			flush();
	
		buffer |= (bit << state);
		state++;
	}


	inline void flush()
	{
		out.put(buffer);
		buffer = 0;
		state = 0;
	}

};

#endif