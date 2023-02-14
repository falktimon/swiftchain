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

Blockchain::Blockchain(long unsigned int try_limit = 10000, 
                       unsigned int difficulty_limit = 100,
                       string user_name = "UNSET", string genesis_data = "",
                       float diff_redux_time = 0.5)
{ 
    this->try_limit = try_limit; 
    this->difficulty_limit = difficulty_limit;
    this->difficulty = 1;

    this->diff_redux_time = (long unsigned int) round(diff_redux_time * 60 * 60 * 1000);

    static Block genesis = Block(genesis_data, user_name);

    if(!this->append_block(&genesis))
        cout << "Could not create Genesis block" << endl;
}

void Blockchain::adjust_difficulty()
{
    /* Adjust difficulty once difficulty limit has been reached. */

    auto r_iter = this->ledger.rbegin();

    long unsigned int time1 = stoi(r_iter->second->get_timestamp());
    long unsigned int time2 = stoi((++r_iter)->second->get_timestamp());

    if((time1 - time2) >= this->diff_redux_time || this->difficulty > 255)
    {
        this->difficulty = (unsigned int) round(this->difficulty / 2);
        return;
    }

    if(!(Blockchain::ledger.size() % this->difficulty_limit))
        this->difficulty++;
}

bool Blockchain::verify_block(Block *block)
{
    /* Verify a given block against the entire ledger. */

    // Return true if block is Genesis:
    if(block->get_block_id() == 0) return true; 

    iter found_block = this->ledger.find(block->get_prev_hash());
    bool is_found = found_block != this->ledger.end();

    // If ledger contains the previous hash, recurse:
    if(is_found) return this->verify_block(this->ledger.at(block->get_prev_hash()));

    //Else, return false
    return false;
}

Block *Blockchain::mine_block(string data, string node_address)
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
                            nonce++, this->get_difficulty());

    } while(!verify_attempt(try_block));

    if(!this->append_block(try_block))
        cout << "Could not append block to chain." << endl;

    this->adjust_difficulty();

    return try_block;
}

void Blockchain::adjust_block_id_and_append(Block *block)
{
    block->set_block_id(Blockchain::get_last_block()->get_block_id() + 1);
    if(!Blockchain::append_block(block))
        cout << "Could not append block with ID " +
        to_string(block->get_block_id()) << " onto ledger." << endl;
}

bool Blockchain::append_block(Block *block)
{
    /* Append a block to the ledger. */
    string block_hash = block->get_block_hash();
    std::pair<std::string, Block *> nextBlock (block_hash, block);

    try 
    { 

        if(!this->verify_block(block))
        {
            cout << "Block with block ID " << block->get_block_id() << " could not be verified." << endl;
            return false;
        }

        Blockchain::ledger.insert(nextBlock); 
        return true; 
    }
    catch(exception &ex)
    {  cout << "Could not append block to chain." << endl; return false; }
}

Block *Blockchain::get_block(string hash)
{
    iter find = this->ledger.find(hash);

    if(find == this->ledger.end())
    {
       cout << "Could not retrieve block with hash " << hash << endl;
       return NULL;
    }

    return this->ledger.at(hash);
}

Block *Blockchain::mine_block_concurrently(string data, string node_address)
{
    Block *nextBlock = NULL;
    unsigned int i = 0;

    while(!nextBlock)
    {
        // If all hardware threads are in use, do nothing
        if(i++ != (thread::hardware_concurrency() - 1))
        {
            auto future = async([this, data, node_address]{ 
                return (Block *) this->mine_block(data, node_address); 
            });

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

bool Blockchain::find_consensus(Blockchain *foreign_chain)
{
    if(count_pow(foreign_chain->get_ledger()) > count_pow(this->ledger))
    {
        this->ledger = foreign_chain->get_ledger();
        return true;
    }

    return false;
}

void Blockchain::set_try_limit(long unsigned int try_limit)
{ this->try_limit = try_limit; }

void Blockchain::set_difficulty_limit(unsigned int difficulyLimit)
{ this->difficulty_limit = difficulty_limit; }

void Blockchain::set_difficulty(long unsigned int difficulty)
{ this->difficulty = difficulty; }

void Blockchain::set_diff_reduction_time(long unsigned int time)
{ 
    time = time * 60 /*minutes*/ * 60 * /*seconds*/ 1000 /*milliseconds*/;
    this->diff_redux_time = time; 
}

Ledger Blockchain::get_ledger()
{ return this->ledger; }

long unsigned int Blockchain::get_difficulty()
{ return this->difficulty; }

long unsigned int Blockchain::get_ledger_size()
{ return (long unsigned int) this->ledger.size(); }

Block *Blockchain::get_last_block()
{ return this->ledger.rbegin()->second; }

unsigned int Blockchain::get_difficulty_limit()
{ return this->difficulty_limit; }

long unsigned int Blockchain::get_try_limit()
{ return this->try_limit; }

long unsigned int Blockchain::get_diff_reduction_time()
{ return this->diff_redux_time; }