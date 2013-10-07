#include <memory>
#include <stdint.h>
#include "trees.h"
#include "binary.h"

std::unique_ptr<Node> Forest::reduce() 
{
	while (trees.size() > 1)
	{
		auto first_tree = std::move(trees.begin()->second);
		trees.erase(trees.begin());
		auto second_tree = std::move(trees.begin()->second);
		trees.erase(trees.begin());

		auto new_tree = merge_trees(std::move(first_tree), std::move(second_tree));
		add(std::move(new_tree));
	}

	return std::move(trees.begin()->second);
}

uint64_t Node::get_binary_code()
{
	uint64_t code = 0;
	
	// Getting lower 55 bits from count to bits 1-55
	uint64_t count_floor = count << 9;
	count_floor >>= 8;
	code |= count_floor;

	// Getting 8 bits representing value to bits 56-63
	uint64_t long_value = value;
	long_value <<= 56;
	code |= long_value;

	// Getting 1 bit representing leaf to bit 0
	if (is_leaf())
	{
		code |= 1;
	}

	return code;
}

void Node::put_binary(std::ofstream& out) 
{
	// Printing node's own code
	uint64_t binary_code = get_binary_code();

	put_int64(out, binary_code);

	// Printing node's descendants
	if (!is_leaf())
	{
		left->put_binary(out);
		right->put_binary(out);
	}
}

std::unique_ptr<Node> merge_trees(std::unique_ptr<Node> first, std::unique_ptr<Node> second) 
{
	uint64_t count = first->get_count() + second->get_count();
	std::unique_ptr<Node> new_root(new Node(0, count, std::move(first), std::move(second)));
	return new_root;
}