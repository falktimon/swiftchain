// Copyright 2020 Falk Spickenbaum
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

//! Construct a Node object
/*! This constructor creates a Node object with a given node identifier.
A node hash is automatically generated from the given node identifier, which may be used
to identify this node in e.g. a hashmap.*/
Node::Node(string node_name)
{
    this->node_name = node_name;
    this->node_address = generate_sha_hash(node_name);
}

//! write_data(string, Blockchain, int)
/*! Parameters:

data: The data to be written into the blockchain, as a string.
chain: The blockchain to be altered.
try_limit (optional): The maximum number of retries to be executed. 

This method provides a higher-level wrapper over the mine_block_concurrently(string, string) 
method from the Blockchain class. It can be used to write data into a given blockchain.
Writing an empty data string is not allowed.
Returns true if a block has been added successfully, else returns false.*/
bool Node::write_data(string data, Blockchain *chain, int try_limit = 10, string meta_data = "")
{
    // If message is empty, return false:
    if(data == "") return false;

    // Try to mine block:
    Block *block = chain->mine_block_concurrently(data, this->node_address);

    // If block is still NULL, retry until try limit has been reached:
    if(!block) 
    {
        if(this->tries++ == try_limit) return false;
        write_data(data, chain, try_limit);
    }

    //! Reset try counter:
    this->tries = 0;

    return true;
}

//! read_data_by_range(unsigned int, Blockchain *)
/*! Parameters: 

range: The number of content strings to be retreived from the ledger.
chain: A Blockchain object from which data should be read.

This method returns a number of content strings from the ledger, as a list in ascending order.
If the range exceeds the size of the ledger, an out-of-bounds exception is thrown.*/
vector<string> Node::read_data_by_range(unsigned int range, Blockchain *chain)
{
    // Get a range of block data from the ledger
    vector<Block *> blocks = chain->get_blocks_by_range(range);
    vector<string> content;

    // Get the data from the blocks:
    for(unsigned int i = 0; i < blocks.size(); i++)
        content.push_back(blocks[i]->get_data());

    return content;
}

//! Get the address (i.e. the SHA256 hash) of the Node object
string Node::get_node_address()
{ return this->node_address; }

//! Get the identifier stored in the Node object
string Node::get_node_name()
{ return this->node_name; }

//! Set the address of this Node object
void Node::set_node_address(string node_address)
{ this->node_address = node_address; }

//! Set the identifier of this node object
void Node::set_node_name(string node_name)
{ this->node_name = node_name; }
