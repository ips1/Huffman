#include "trees.h"
#include "binary.h"
#include "huffman.h"
#include "encode.h"
#include <iostream>

int main(int argc, char** argv)
{
	std::unique_ptr<Node> node1(new Node(1,25,NULL,NULL));
	std::unique_ptr<Node> node2(new Node(2,15,NULL,NULL));
	std::unique_ptr<Node> node3(new Node(3,5,NULL,NULL));
	std::unique_ptr<Node> node4(new Node(4,150,NULL,NULL));

	Forest f;

	f.add(std::move(node1));
	f.add(std::move(node2));
	f.add(std::move(node3));
	f.add(std::move(node4));
	
	f.reduce();

	uint64_t n = 123456789;
	std::ofstream output("test", std::ios::binary);

	put_int64(output, n);

	output.close();

	std::ifstream input("test", std::ios::binary);

	std::cout << get_int64(input) << std::endl;


	std::cout << "TEST" << std::endl;

	std::ifstream in("test.avi", std::ios::binary);

	std::ofstream out("test.avi.encoded", std::ios::binary);

	encode(in, out);

	std::cout << "TEST" << std::endl;

}