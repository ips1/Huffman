#include "encode.h"

void encode(std::ifstream& in, std::ofstream& out)
{
	Huffman huffman_context = build_huffman(in);

	huffman_context.generate_encode_table();

	in.clear();

	in.seekg(0, std::ios::beg);

	huffman_context.encode_stream(in, out);
}