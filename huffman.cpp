#include "huffman.h"
#include "binary.h"
#include <iostream>

Huffman build_huffman(std::ifstream& in)
{
	// Counting all 2^8 bytes in input stream
	ByteCounter counter;
	while (in.good())
	{
		uint8_t byte = in.get();
		if (in.good())
			counter.add(byte);
	}

	// All bytes have been counted, building a tree
	Forest f;
	for (int i = 0; i <= UINT8_MAX; i++)
	{
		std::unique_ptr<Node> simple_tree(new Node(i, counter.get_count(i), NULL, NULL));
		f.add(std::move(simple_tree));
	}

	std::unique_ptr<Node> root = f.reduce();
	
	Huffman result(std::move(root));

	// MSVC CALLING COPYCTOR HERE! EPIC FAIL! (GCC OK)
	// Probably because it doesn't generate default movector
	return result;
}


void Huffman::generate_encode_table()
{
	std::vector<uint8_t> code;
	std::vector<Node*> path;
	Node* current_node;

	current_node = root.get();

	path.push_back(current_node);

	while (!path.empty())
	{
		int n = path.size();
		current_node = path.back();

		// We are in a leaf
		if (current_node->is_leaf())
		{
			uint8_t byte = current_node->get_value();
			// Copying the encoded path
			for (auto it = code.begin(); it < code.end(); it++)
			{
				encode_table[byte].push_back(*it);
			}
			// Returning up the tree
			path.pop_back();
			continue;
		}

		// We are in an inner node, we could have come from 3 directions
		if (code.size() < n)
		{
			// We have come from the parent, continue to the left child
			code.push_back(0);
			path.push_back(current_node->get_left().get());
			continue;
		}
		if (code.back() == 0)
		{
			// We have come from the left son, continue to the right one
			code.pop_back();
			code.push_back(1);
			path.push_back(current_node->get_right().get());
			continue;
		}
		if (code.back() == 1)
		{
			// We have come from the right son, continue back up
			code.pop_back();
			path.pop_back();
			continue;
		}
	}

	encode_table_generated = true;
}

void Huffman::write_header(std::ofstream& out)
{
	for (int i = 0; i < 8; i++)
	{
		out.put(header[i]);
	}
}

void Huffman::write_tree(std::ofstream& out)
{
	root->put_binary(out);

	put_int64(out, 0);

}

inline void Huffman::encode_byte(BitStream& bit_stream, uint8_t byte)
{
	std::vector<uint8_t>& code = encode_table[byte];
	for (auto it = code.begin(); it < code.end(); it++)
	{
		bit_stream.print_bit(*it);
	}
}

void Huffman::encode_stream(std::ifstream& in, std::ofstream& out)
{
	if (!encode_table_generated)
	{
		generate_encode_table();
	}

	write_header(out);
	write_tree(out);

	BitStream bit_stream(out);

	uint64_t progress = 0;
	uint8_t byte = in.get();

	std::cout << "Encoding stream" << std::endl;
	while (in.good())
	{

		/*progress++;
		if (progress % 1000000 == 0)
			std::cout << "Progress: " << progress / 1000000 << "MB." << std::endl;*/

		encode_byte(bit_stream, byte);
		byte = in.get();
	}

	bit_stream.flush();
}