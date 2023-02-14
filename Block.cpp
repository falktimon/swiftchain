#ifndef BLOCK_HEADER
#define BLOCK_HEADER
#include "Block.hpp"
#endif

#ifndef HASH_HEADER
#define HASH_HEADER
#include "HashingUtil.hpp"
#endif

using namespace std;

//! Block constructor
/*! Parameters:

prev: An instance of a Block object. 
data: Some content to be contained within the Block.
userAddress: The nodeAddress of the Node creating this block.
nonce: A number used to generate varying hashes.
difficulty: The current difficulty level. 

This is the common constructor used for mining blocks.*/
Block::Block(Block *prev, string data, string userAddress,
            long unsigned int nonce, long unsigned int difficulty)
{
    /* Common constructor. Used in mining blocks, for e.g. data uploading.*/

    this->blockID = (*prev).blockID + 1;
    this->data = data;
    this->nodeAddress = userAddress;
    this->timestamp = generateTimestamp();

    this->difficulty = difficulty;
    this->nonce = nonce;

    this->prevHash = prev->getBlockHash();
    this->blockHash = generateBlockHash(this);
}

//! Genesis constructor
/*! Parameters: 

data -> Some content to be written into the Genesis block.
nodeAddress -> The hash string of the Node that is creating the Genesis block

A Genesis constructor, used to start a new blockchain. This constructor is called in the 
Blockchain constructor. The Genesis block is used to validate any consecutive block in the blockchain. */
Block::Block(string data, string nodeAddress)
{
    /* Genesis constructor.*/

    this->blockID = 0;
    this->data = data;
    this->nodeAddress = nodeAddress;

    this->timestamp = generateTimestamp();
    this->difficulty = 0;
    this->nonce = 0;

    this->prevHash = "";

    this->blockHash = generateBlockHash(this);
}

string Block::generateTimestamp()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    return string(to_string(now_c));
}

string Block::generateBlockHash(Block *b)
{
    // Turn the attributes of the Block object into a single string:
    string seed = b->data + b->nodeAddress + b->timestamp +
                    to_string(b->difficulty) + to_string(b->nonce) +
                    b->prevHash; 

    // Return the hash of the assembled string:
    return generateSHAHash<string>(seed);
}

//! getBlockID(void)
/*! Parameters: None
Returns the unique ID of the Block object.*/
long unsigned int Block::getBlockID()
{ return this->blockID; }

//! getData(void)
/*! Parameters: None
Get the content container within the Block object. */
string Block::getData()
{ return this->data; }

//! getBlockHash(void)
/*! Parameters: None
Get the hash of the Block object.*/
string Block::getBlockHash()
{ return this->blockHash; }

//! getPrevHash(void)
/*! Parameters: None
Get the previous hash stored within in Block object. */
string Block::getPrevHash()
{ return this->prevHash; }

//! getDifficulty(void)
/*! Parameters: None
Get the difficulty level under which the current block was mined.*/
long unsigned int Block::getDifficulty()
{ return this->difficulty; }

//! getNonce(void)
/*! Parameters: None
Return the nonce with which the Block object was mined.*/
long int Block::getNonce()
{ return this->nonce; }

//! setBlockID(long unsigned int)
/*! Parameters: A unique integer to be used as a block identifier.*/
void Block::setBlockID(long unsigned int blockID)
{ this->blockID = blockID; }

//! getNodeAddress(void)
/*! Parameters: None
Get the address of the node that mined this block.*/
string Block::getNodeAddress()
{ return this->nodeAddress; }

string Block::getTimestamp()
{ return this->timestamp; }
