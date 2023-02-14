#ifndef BLOCK_HEADER
#define BLOCK_HEADER
#include "block.hpp"
#endif

#ifndef HASH_HEADER
#define HASH_HEADER
#include "hashing_util.hpp"
#endif

using namespace std;

//! Construct a common Block object
/*! Parameters:

prev: An instance of a Block object. 
data: Some content to be contained within the Block.
userAddress: The node_address of the Node creating this block.
nonce: A number used to generate varying hashes.
difficulty: The current difficulty level. 

This is the common constructor used for mining blocks.*/
Block::Block(Block *prev, string data, string userAddress,
            long unsigned int nonce, long unsigned int difficulty,
            string blockchain_id = "0")
{
    /* Common constructor. Used in mining blocks, for e.g. data uploading.*/

    this->block_id = (*prev).block_id + 1;
    this->data = data;
    this->node_address = userAddress;
    this->timestamp = generate_timestamp();
    this->blockchain_id = blockchain_id;

    this->difficulty = difficulty;
    this->nonce = nonce;

    this->prev_hash = prev->get_block_hash();
    this->block_hash = generate_block_hash(this);
}

//! Construct a Genesis block
/*! Parameters: 

data -> Some content to be written into the Genesis block.
node_address -> The hash string of the Node that is creating the Genesis block

A Genesis constructor, used to generate a Genesis block. This constructor is called in the 
Blockchain constructor. The Genesis block is used to validate any consecutive block in the blockchain. */
Block::Block(string data, string node_address)
{
    /* Genesis constructor.*/

    this->block_id = 0;
    this->data = data;
    this->node_address = node_address;

    this->timestamp = generate_timestamp();
    this->difficulty = 0;
    this->nonce = 0;

    this->prev_hash = "";

    this->block_hash = generate_block_hash(this);
}

//! generate_timestamp(void)
/*! Parameters: None
Returns a UNIX timestamp as a string. */
string Block::generate_timestamp()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    return string(to_string(now_c));
}

//! generate_block_hash(Block *)
/*! Parameters: Some Block object to be hashed.
Generate a SHA256 hash string from a given Block object.*/
string Block::generate_block_hash(Block *b)
{
    // Turn the attributes of the Block object into a single string:
    string seed = b->data + b->node_address + b->timestamp +
                    to_string(b->difficulty) + to_string(b->nonce) +
                    b->prev_hash + b->blockchain_id + to_string(b->block_id); 

    // Return the hash of the assembled string:
    return generate_sha_hash(seed);
}

//! get_block_id(void)
/*! Parameters: None
Returns the unique ID of the Block object as an integer.*/
long unsigned int Block::get_block_id()
{ return this->block_id; }

//! get_data(void)
/*! Parameters: None
Get the content contained within the Block object. */
string Block::get_data()
{ return this->data; }

//! get_block_hash(void)
/*! Parameters: None
Get the hash of the Block object.*/
string Block::get_block_hash()
{ return this->block_hash; }

//! get_prev_hash(void)
/*! Parameters: None
Get the previous hash stored within in Block object. */
string Block::get_prev_hash()
{ return this->prev_hash; }

//! get_difficulty(void)
/*! Parameters: None
Get the difficulty level under which the current block was mined.*/
long unsigned int Block::get_difficulty()
{ return this->difficulty; }

//! get_nonce(void)
/*! Parameters: None
Return the nonce with which the Block object was mined.*/
long int Block::get_nonce()
{ return this->nonce; }

//! set_block_id(long unsigned int)
/*! Parameters: A unique integer to be used as a block identifier.*/
void Block::set_block_id(long unsigned int block_id)
{ this->block_id = block_id; }

//! set_blockchain_id(string)
/*! Parameters: A blockchain ID as a string.
Specify a blockchain which this Block object is contained with.*/
void Block::set_blockchain_id(string blockchain_id)
{ this->blockchain_id = blockchain_id; }

//! set_node_address(string)
/*! Parameters: A Node address as a string.
Set the Node address associated with this Block object.*/
void Block::set_node_address(string node_address)
{ this->node_address = node_address; }

//! getnode_address(void)
/*! Parameters: None
Get the address of the node that mined this block.*/
string Block::get_node_address()
{ return this->node_address; }

//! get_timestamp(void)
/*! Parameters: None
Get the UNIX timestamp contained within this string*/
string Block::get_timestamp()
{ return this->timestamp; }

//! get_blockchain_id(void)
/*! Get the ID of the blockchain associated with this Block object. */
string Block::get_blockchain_id()
{ return this->blockchain_id; }

//! set_data(string)
/*! Set the content of this Block object.*/
void Block::set_data(string data)
{ this->data = data; }
