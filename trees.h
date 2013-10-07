#ifndef TREES_H
#define TREES_H


#include <memory>
#include <stdint.h>
#include <map>
#include <fstream>

struct NodeOrder
{
	uint64_t count;
	uint8_t value;
	int order;
	bool is_leaf;

	NodeOrder(uint64_t _count, uint8_t _value, int _order, bool _is_leaf): count(_count), value(_value), order(_order), is_leaf(_is_leaf) {}
};

struct NodeComparer
{
	bool operator()(const NodeOrder& left, const NodeOrder& right)
	{
		if (left.count < right.count)
        {
            return true;
        }
        else if (left.count > right.count)
        {
            return false;
        }
        else
        {
			if (left.is_leaf && (!right.is_leaf))
			{
				return true;
			}
			else if ((!left.is_leaf) && right.is_leaf)
            {
                return false;

            }
			else if (left.is_leaf && right.is_leaf)
            {
				return left.value < right.value;
            }
            else
            {
				return left.order < right.order;
            }

        }
	}
};

// Class representing one node in a Huffmann binary tree built from all bytes contained in source file
class Node 
{
private:
	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;

	uint8_t value;
	uint64_t count;

public: 
	// Constructor taking pointers to both children, they have to be moved to new owner
	Node(uint8_t v, uint64_t c, std::unique_ptr<Node> l, std::unique_ptr<Node> r) : value(v), count(c), left(std::move(l)), right(std::move(r)) { }
	
	// Returns true if the node is leaf in the tree
	bool is_leaf() 
	{
		return ((left == NULL) && (right == NULL));
	}

	uint8_t get_value() 
	{
		return value;
	}

	uint64_t get_count() 
	{
		return count;
	}

	std::unique_ptr<Node>& get_left() 
	{
		return left;
	}

	std::unique_ptr<Node>& get_right() 
	{
		return right;
	}

	// Returns 64bit binary code representing the node (for printing into binary file)
	uint64_t get_binary_code();

	// Puts binary representation of the node and all its descendants to given stream
	void put_binary(std::ofstream& out);
};

class Forest
{
private:
	std::multimap<NodeOrder, std::unique_ptr<Node>, NodeComparer> trees;
	int current_number;

public:
	Forest()
	{
		current_number = 0;
	}

	void add(std::unique_ptr<Node> tree)
	{
		trees.insert(std::make_pair(NodeOrder(tree->get_count(), tree->get_value(), current_number++, tree->is_leaf()), std::move(tree)));
	}

	std::unique_ptr<Node> reduce();
};

std::unique_ptr<Node> merge_trees(std::unique_ptr<Node> first, std::unique_ptr<Node> second);



#endif