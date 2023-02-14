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

#ifndef BLOCKCHAIN_HEADER
#define BLOCKCHAIN_HEADER
#include "blockchain.hpp"
#endif 

#ifndef POW_HEADER
#define POW_HEADER
#include "pow_util.hpp"
#endif 

#ifndef BLOCK_CPP
#define BLOCK_CPP
#include "block.cpp"
#endif

#include <iostream>
#include <cmath>

using namespace std;

typedef map<string, Block *>::const_iterator iter;

//! Construct a Blockchain object
/*! Parameters:

try_limit: The maximum number of hashes generated for each mining attempt.
difficulty_limit: The number of Block objects in the ledger at which the difficulty is incremented.
node_addr: Some user identifier, as a string. 
genesis_data: The data to be stored in the Genesis block.
diff_redux_time: The timespan in hours that has to pass between Blocks in order for the difficulty to be halfed.

Construct a Blockchain object. All parameters for this constructor have default values. 
node_addr and genesis_data should always be supplied.*/
Blockchain::Blockchain(long unsigned int try_limit = 10000, 
                       unsigned int difficulty_limit = 100,
                       string node_addr = "UNSET", string genesis_data = "",
                       float diff_redux_time = 0.5)
{ 
    this->try_limit = try_limit; 
    this->difficulty_limit = difficulty_limit;
    this->difficulty = 1;

    // Convert hours to milliseconds:
    this->diff_redux_time = (long unsigned int) round(diff_redux_time * 60 * 60 * 1000);

    // Create a Genesis block:
    static Block genesis = Block(genesis_data, node_addr);
    genesis.set_blockchain_id(genesis.get_block_hash());
    
    // Turn the Genesis block hash into the Blockchain ID:
    this->blockchain_id = genesis.get_block_hash();

    // Try to append Genesis block to ledger:
    if(!this->append_block(&genesis))
        cout << "Could not create Genesis block" << endl;
}

//! get_blocks_by_range(unsigned int)
/*! Parameters: The number of blocks to be retrieved from the ledger.
Returns a range of Blocks from the ledger as a vector. If the range exceeds the size of
the ledger, an out_of_bounds exception is thrown. Blocks are ordered by ID in ascending order.*/
vector<Block *> Blockchain::get_blocks_by_range(unsigned int range)
{
    vector<Block *> block_range;
    iter l_iter = this->ledger.begin();

    // Check if range exceeds size of ledger:
    if(range > this->ledger.size())
        throw std::out_of_range("Requested range exceeds size of ledger.");

    // Start with the last Block in the ledger:
    block_range.push_back(this->get_last_block());
    long unsigned int largest_id = block_range[0]->get_block_id();

    // TODO: Replace this bad, O(n^2) sorting algorithm:
    while(block_range.size() != range)
    {
        if(l_iter == this->ledger.end())
            l_iter = this->ledger.begin();

        if(l_iter->second->get_block_id() == (largest_id - 1))
        {
            block_range.push_back(l_iter->second);
            largest_id = l_iter->second->get_block_id();
        }

        l_iter++;
    }

    // Reverse the vector that was retrieved:
    reverse(begin(block_range), end(block_range));

    return block_range;
}

//! adjust_difficulty(void)
/*! Parameters: None
Checks if the conditions for adjusting the difficulty of the Blockchain are met and acts accordingly. */
void Blockchain::adjust_difficulty()
{
    /* Adjust difficulty once difficulty limit has been reached. */

    auto r_iter = this->ledger.rbegin();

    // Get the UNIX timestamp stored in the last block and the second to last block:
    long unsigned int time1 = stoi(r_iter->second->get_timestamp());
    long unsigned int time2 = stoi((++r_iter)->second->get_timestamp());

    // If time difference is greater than redux time, half difficulty:
    if((time1 - time2) >= this->diff_redux_time || this->difficulty > 255)
    {
        this->difficulty = (unsigned int) round(this->difficulty / 2);
        return;
    }

    // If number of block matches difficulty limit, increment difficulty:
    if(!(Blockchain::ledger.size() % this->difficulty_limit))
        this->difficulty++;
}

//! verify_block(Block *)
/*! Parameters: Any block object to be validated. 
Tries to verify a given block against the whole blockchain.
Returns true on success, false on failure.*/
bool Blockchain::verify_block(Block *block)
{
    /* Verify a given block against the entire ledger. */

    // Return false if block was not mined on this chain:
    if(block->get_blockchain_id() != this->blockchain_id) 
        return false;

    // Return true if block is Genesis:
    if(block->get_block_id() == 0) return true;

    iter found_block = this->ledger.find(block->get_prev_hash());
    bool is_found = found_block != this->ledger.end();

    // If ledger contains the previous hash, recurse:
    if(is_found) return this->verify_block(this->ledger.at(block->get_prev_hash()));

    //Else, return false
    return false;
}

//! mine_block(string, string)
/* Parameters: 

data: Some data to be stored in the new block.
node_address: Some user identifier as a string.

This method tries to mine a Block object on this Blockchain. This method does not take
advantage of multi-processing. If a block should be mined in parallel, 
use the mine_block_concurrently(string, string) method.
Appends a new Block to the ledger and returns it on success, NULL on failure.*/
Block *Blockchain::mine_block(string data, string node_address, string meta_data = "")
{
    /* Mine a new block by solving a proof-of-work puzzle. */

    int nonce = 0;
    Block *try_block = NULL;
    unsigned long tries = 0;

    do
    {
        // Check if try limit has been exceeded
        if(tries++ == this->try_limit) return NULL;

        // Create a block with the next nonce
        try_block = new Block(this->get_last_block(), data, node_address,
                            nonce++, this->get_difficulty(), this->blockchain_id,
                            meta_data);

    } while(!verify_attempt(try_block));

    // Try and append/verify the new block on the blockchain
    if(!this->append_block(try_block))
        cout << "Could not append block to chain." << endl;

    // Adjust difficulty after new block has been mined:
    this->adjust_difficulty();

    return try_block;
}

//! adjust_block_id_and_append(Block *)
/*! Parameters: Some Block object.
An internal method that ensures that a block's ID does not clash with any ID already stored in the ledger.*/
void Blockchain::adjust_block_id_and_append(Block *block)
{
    // Set the block ID relative to the last block stored in the ledger:
    block->set_block_id(Blockchain::get_last_block()->get_block_id() + 1);
    // Try to append block to the chain:
    if(!Blockchain::append_block(block))
        cout << "Could not append block with ID " +
        to_string(block->get_block_id()) << " onto ledger." << endl;
}

//! append_block(Block *)
/*! Parameters: Some Block object to be append to the ledger.
Tries to append a Block object into the ledger.
Returns true on success and false on failure, i.e. if the Block could not be verified.*/
bool Blockchain::append_block(Block *block)
{
    /* Append a block to the ledger. */
    string block_hash = block->get_block_hash();
    std::pair<std::string, Block *> nextBlock (block_hash, block);

    try 
    { 
        // Try to verify the block:
        if(!this->verify_block(block))
        {
            cout << "Block with block ID " << block->get_block_id() << " could not be verified." << endl;
            return false;
        }

        // Try to insert the block-string pair into the ledger:
        Blockchain::ledger.insert(nextBlock); 
        return true; 
    }
    catch(exception &ex)
    {  cout << "Could not append block to chain." << endl; return false; }
}

//! get_block(string)
/*! Parameters: A block hash as a string.
Find a Block object in the ledger by hash.
Returns a Block object on success, NULL on failure.*/
Block *Blockchain::get_block(string hash)
{
    // Get an iterator to the hash in question:
    iter find = this->ledger.find(hash);

    // If ledger does not contain the hash, return NULL
    if(find == this->ledger.end())
    {
       cout << "Could not retrieve block with hash " << hash << endl;
       return NULL;
    }

    // Return the Block in question:
    return this->ledger.at(hash);
}

//! get_block_by_index(unsigned int)
/*! Parameters: The index of the relevant Block object in the Blockchain

Get the Block object at the specified index in the ledger.*/
Block *Blockchain::get_block_by_index(unsigned int index)
{
    // Throw out of range exception if index is too large:
    if(index > this->get_ledger_size())
        throw out_of_range("Requested index exceeds size of ledger.");

    // Get all blocks from ledger:
    vector<Block *> blocks = this->get_blocks_by_range(this->get_ledger_size());
    // Return block at [index]
    return blocks[index];
}


//! mine_block_concurrently(string, string)
/*! Parameters:

data: The data to be stored in the new block, as a string.
node_address: A string identifier of a blockchain user. 

This method tries to mine a single block in various threads. 
If an attempt should be started on a single thread, use mine_block(string, string).*/
Block *Blockchain::mine_block_concurrently(string data, string node_address, string meta_data = "")
{
    Block *nextBlock = NULL;

    // Try until block has been found:
    while(!nextBlock)
    {
        // If all hardware thread contexts are in use, do nothing
        if(thread::hardware_concurrency() - 1)
        {   
            // Start a thread using a future:
            auto future = async([this, data, node_address, meta_data]{ 
                return (Block *) this->mine_block(data, node_address, meta_data); 
            });

            // Get the value from the future and continue:
            nextBlock = future.get(); 
        }
    }

    return nextBlock;
}

long unsigned int count_pow(Ledger l)
{
    long unsigned int cml_pow = 0;
    iter i = l.begin();

    do cml_pow += (long unsigned int) pow(2, i->second->get_difficulty());
    while(++i != l.end());

    return cml_pow;
}

//! find_consensus(Blockchain *)
/*! Parameters: Another blockchain object to be used in the consensus algorithm.
This method attempts to find a consensus between to ledgers by comparing the cumulative 
proof-of-work contained in the ledger.
If the foreign blockchain contains more cumulative proof-of-work, the ledger of this Blockchain object 
is replaced. If this is the case, this case returns true. Else it returns false.*/
bool Blockchain::find_consensus(Blockchain *foreign_chain)
{
    // Compare the cumulative proof-of-work contained within the ledger:
    if(count_pow(foreign_chain->get_ledger()) > count_pow(this->ledger))
    {
        this->ledger = foreign_chain->get_ledger();
        return true;
    }

    return false;
}

//! Set the maximum number of hashes to be generated per mining attempt
void Blockchain::set_try_limit(long unsigned int try_limit)
{ this->try_limit = try_limit; }

//! Set the number of blocks contained with the ledger at which the difficulty is raised:
void Blockchain::set_difficulty_limit(unsigned int difficulty_limit)
{ this->difficulty_limit = difficulty_limit; }

//! Set the current difficulty of the blockchain
void Blockchain::set_difficulty(long unsigned int difficulty)
{ this->difficulty = difficulty; }

//! Set the timespan in hours after which the difficulty is halfed
void Blockchain::set_redux_time(float time)
{ 
    time = round(time * 60 /*minutes*/ * 60 * /*seconds*/ 1000 /*milliseconds*/);
    this->diff_redux_time = (long unsigned int) time; 
}

//! Return the ledger contained in the Blockchain object
Ledger Blockchain::get_ledger()
{ return this->ledger; }

//! Get the current difficulty contained within the Blockchain object
long unsigned int Blockchain::get_difficulty()
{ return this->difficulty; }

//! Get the current size of the ledger
long unsigned int Blockchain::get_ledger_size()
{ return (long unsigned int) this->ledger.size(); }

//! Get the last block in the ledger, meaning the block with the highest block ID
Block *Blockchain::get_last_block()
{ 
    iter r = this->ledger.begin();
    Block *last_block = r->second;

    // Find the block with the highest block ID:
    while(++r != this->ledger.end())
    {
        if(r->second->get_block_id() > last_block->get_block_id())
            last_block = r->second;
        
    }

    return last_block; 
}

//! Get the current number of Blocks at which the difficulty is raised
unsigned int Blockchain::get_difficulty_limit()
{ return this->difficulty_limit; }

//! Get the maximum number of hashes generated per mining attempt
long unsigned int Blockchain::get_try_limit()
{ return this->try_limit; }

//! Get the timespan after which the difficulty is halfed:
long unsigned int Blockchain::get_redux_time()
{ return this->diff_redux_time; }

//! Get the identifier of the Blockchain object:
string Blockchain::get_blockchain_id()
{ return this->blockchain_id; }