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
threads: The number of threads to be used in mining. 

This method provides a higher-level wrapper over the mine_block_concurrently(string, string) 
method from the Blockchain class. It can be used to write data into a given blockchain.
Returns true if a block has been added successfully, else returns false.*/
bool Node::write_data(string data, Blockchain *chain, int try_limit = 20, 
                      string meta_data = "", unsigned int threads = 5)
{
    Block *block = NULL;

    // Try to mine block:
    if(threads == 0 || threads == 1) block = chain->mine_block(data, this->node_address, meta_data);
    else block = chain->mine_block_concurrently(data, this->node_address, meta_data, threads);

    // If block is still NULL, retry until try limit has been reached:
    if(!block) 
    {
        if(this->tries++ >= try_limit) return false;
        this->write_data(data, chain, try_limit, meta_data);
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

    if(range > chain->get_ledger_size())
        throw out_of_range("Requested range exceeds size of ledger.");

    // Get the data from the blocks:
    for(unsigned int i = 0; i < blocks.size(); i++)
        content.push_back(blocks[i]->get_data());

    return content;
}

//! get_blocks_by_meta(string, Blockchain *)
/*! Parameters: 

meta: A metadata string
chain: A Blockchain object

Get a list of Blocks which share a common metadata attribute.*/
vector<Block *> Node::get_blocks_by_meta(string meta, Blockchain *chain)
{
    // Get all blocks in ledger, ordered by block ID:
    vector<Block *> blocks = chain->get_blocks_by_range(chain->get_ledger_size());
    vector<Block *> content;

    // Search blocks retrieved above for the supplied metadata tag:
    for(unsigned int i = 0; i < blocks.size(); i++)
    {
        if(blocks[i]->get_meta_data() == meta)
            content.push_back(blocks[i]);
    }

    // If no Blocks were found, throw exception:
    if(content.empty())
        throw out_of_range("Ledger does not contain block with meta tag " + meta);

    return content;
}

//! read_data_by_meta(string, Blockchain *)
/*! Parameters: 

meta: A metadata string
chain: A Blockchain object

Get the data from all Block objects which share a common metadata attribute.*/
vector<string> Node::read_data_by_meta(string meta, Blockchain *chain)
{
    // Get all blocks in ledger, ordered by block ID:
    vector<Block *> blocks = chain->get_blocks_by_range(chain->get_ledger_size());
    vector<string> content;

    // Search blocks retrieved above for the supplied 
    // metadata tag and save their content:
    for(unsigned int i = 0; i < blocks.size(); i++)
    {
        if(blocks[i]->get_meta_data() == meta)
            content.push_back(blocks[i]->get_data());
    }

    // If no Blocks were found, throw exception:
    if(content.empty())
        throw out_of_range("Ledger does not contain block with meta tag " + meta);

    return content;
}

//! get_block_by_index(unsigned int, Blockchain *)
/*! Parameters: 

index: The index of the relevant block in the Blockchain
chain: A Blockchain object

Get the Block object at the specified index in the ledger.*/
Block *Node::get_block_by_index(unsigned int index, Blockchain *chain)
{
    if(index > chain->get_ledger_size())
        throw out_of_range("Requested index exceeds size of ledger.");

    vector<Block *> blocks = chain->get_blocks_by_range(chain->get_ledger_size());

    return blocks[index];
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
