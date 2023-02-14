#ifndef HASH_HEADER
#define HASH_HEADER
#include "hashing_util.hpp"
#endif

#ifndef BLOCKCHAIN_CPP
#define BLOCKCHAIN_CPP
#include "blockchain.cpp"
#endif

#ifndef NODE_HEADER
#define NODE_HEADER
#include "node.hpp"
#endif

#include <iostream>

using namespace std;

typedef reverse_iterator<_Rb_tree_iterator<pair<const string, Block *>>> r_iter;

Node::Node(string node_name)
{
    this->node_name = node_name;
    this->node_address = generate_sha_hash(node_name);
}

bool Node::write_data(string data, Blockchain *chain, int tryLimit = 10)
{
    if(data == "") return false;

    Block *block = chain->mine_block_concurrently(data, this->node_address);

    if(!block) 
    {
        if(this->tries++ == tryLimit) return false;
        write_data(data, chain, tryLimit);
    }

    this->tries = 0;
    return true;
}

vector<string> Node::read_data_by_range(unsigned int range, Blockchain *chain)
{
    // Get a range of block data from the ledger
    vector<Block *> blocks = chain->get_blocks_by_range(range);
    vector<string> content;

    for(unsigned int i = 0; i < blocks.size(); i++)
        content.push_back(blocks[i]->get_data());

    return content;
}

string Node::get_node_address()
{ return this->node_address; }

string Node::get_node_name()
{ return this->node_name; }

void Node::set_node_address(string node_address)
{ this->node_address = node_address; }

void Node::set_node_name(string node_name)
{ this->node_name = node_name; }
