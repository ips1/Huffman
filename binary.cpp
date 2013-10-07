#include "binary.h"
	
// Functions for printing longer numbers into binary files
// Independent on platform (little / big endian)
// Data from files have to be read by equivalent reading functions

void put_int64(std::ofstream& out, uint64_t number)
{
	for (int i = 0; i < 8; i++)
	{
		uint8_t floor = static_cast<uint8_t>(number & 0xFFu);
		out.put(floor);
		number >>= 8;
	}
}


uint64_t get_int64(std::ifstream& in)
{
	uint64_t result = 0;
	for (int i = 0; i < 4; i++)
	{
		uint8_t floor = in.get();
		uint64_t moved = floor;
		moved <<= (i * 8);
		result |= moved;
	}
	return result;
}


