#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "trees.h"
#include <stdint.h>
#include <vector>
#include <fstream>
#include "binary.h"

// Class for storing counts of all existing bytes (for analysis of a file byte by byte)
class ByteCounter
{
private:
	std::vector<uint64_t> counts;

public:
	ByteCounter() 
	{
		for (int i = 0; i <= UINT8_MAX; i++)
		{
			counts.push_back(0);
		}
	}

	// Adds one to the counter of specified byte
	void add(uint8_t byte)
	{
		counts[byte]++;
	}

	// Gets the count of specified byte
	uint64_t get_count(uint8_t byte)
	{
		return counts[byte];
	}
};

// Class for storing Huffman data and for encoding the file
class Huffman
{
private:
	uint8_t header[8];
	std::unique_ptr<Node> root;
	std::vector<uint8_t> encode_table[UINT8_MAX + 1];

	bool encode_table_generated;

	// Function for Huffman construction (constructor is private)
	friend Huffman build_huffman(std::ifstream& in);
	
	void write_header(std::ofstream& out);
	void write_tree(std::ofstream& out);
	void encode_byte(BitStream& bit_stream, uint8_t byte);

	Huffman(std::unique_ptr<Node> _root): root(std::move(_root))
	{
		header[0] = 0x7B;
        header[1] = 0x68;
        header[2] = 0x75;
        header[3] = 0x7C;
        header[4] = 0x6D;
        header[5] = 0x7D;
        header[6] = 0x66;
        header[7] = 0x66;
		encode_table_generated = false;
	}

public:
	void generate_encode_table();
	void encode_stream(std::ifstream& in, std::ofstream& out);

	// USER DEFINED MOVECTOR BECAUSE OF MSVC (NOT GENERATING DEFAULT ONE!!!)

	Huffman(Huffman&& _old): root(std::move(_old.root))
	{
		header[0] = 0x7B;
        header[1] = 0x68;
        header[2] = 0x75;
        header[3] = 0x7C;
        header[4] = 0x6D;
        header[5] = 0x7D;
        header[6] = 0x66;
        header[7] = 0x66;
		encode_table_generated = _old.encode_table_generated;
	}

};

Huffman build_huffman(std::ifstream& in);


#endif