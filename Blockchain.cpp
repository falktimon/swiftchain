#ifndef BLOCKCHAIN_HEADER
#define BLOCKCHAIN_HEADER
#include "Blockchain.hpp"
#endif 

#ifndef POW_HEADER
#define POW_HEADER
#include "PoWUtil.hpp"
#endif 

#ifndef BLOCK_CPP
#define BLOCK_CPP
#include "Block.cpp"
#endif

#include <iostream>
#include <cmath>

using namespace std;

typedef map<string, Block *>::const_iterator iter;

Blockchain::Blockchain(long unsigned int tryLimit = 10000, 
                       unsigned int difficultyLimit = 100,
                       string userName = "UNSET", string genesisData = "",
                       float diffReductionTime = 0.5)
{ 
    this->tryLimit = tryLimit; 
    this->difficultyLimit = difficultyLimit;
    this->difficulty = 1;

    this->diffReductionTime = (long unsigned int) round(60 * 1000 * diffReductionTime);

    static Block genesis = Block(genesisData, userName);

    if(!this->appendBlock(&genesis))
        cout << "Could not create Genesis block" << endl;
}

void Blockchain::adjustDifficulty()
{
    /* Adjust difficulty once difficulty limit has been reached. */

    auto rIter = this->ledger.rbegin();

    long unsigned int time1 = stoi(rIter->second->getTimestamp());
    long unsigned int time2 = stoi((++rIter)->second->getTimestamp());

    if((time1 - time2) >= this->diffReductionTime || this->difficulty > 255)
    {
        this->difficulty = (unsigned int) round(this->difficulty / 2);
        return;
    }

    if(!(Blockchain::ledger.size() % this->difficultyLimit))
        this->difficulty++;
}

bool Blockchain::verifyBlock(Block *block)
{
    /* Verify a given block against the entire ledger. */

    // Return true if block is Genesis:
    if(block->getBlockID() == 0) return true; 

    iter foundBlock = this->ledger.find(block->getPrevHash());
    bool isFound = foundBlock != this->ledger.end();

    // If ledger contains the previous hash, recurse:
    if(isFound) return this->verifyBlock(this->ledger.at(block->getPrevHash()));

    //Else, return false
    return false;
}

Block *Blockchain::mineBlock(string data, string nodeAddress)
{
    /* Mine a new block by solving a proof-of-work puzzle. */

    int nonce = 0;
    Block *tryBlock = NULL;
    unsigned long tries = 0;

    do
    {
        // Check if try limit has been exceeded
        if(tries++ == this->tryLimit) return NULL;

        // Create a block with the next nonce
        tryBlock = new Block(this->getLastBlock(), data, nodeAddress,
                            nonce++, this->getDifficulty());

    } while(!verifyAttempt(tryBlock));

    if(!this->appendBlock(tryBlock))
        cout << "Could not append block to chain." << endl;

    this->adjustDifficulty();

    return tryBlock;
}

void Blockchain::adjustBlockIDAndAppend(Block *block)
{
    block->setBlockID(Blockchain::getLastBlock()->getBlockID() + 1);
    if(!Blockchain::appendBlock(block))
        cout << "Could not append block with ID " +
        to_string(block->getBlockID()) << " onto ledger." << endl;
}

bool Blockchain::appendBlock(Block *block)
{
    /* Append a block to the ledger. */
    string blockHash = block->getBlockHash();
    std::pair<std::string, Block *> nextBlock (blockHash, block);

    try 
    { 

        if(!this->verifyBlock(block))
        {
            cout << "Block with block ID " << block->getBlockID() << " could not be verified." << endl;
            return false;
        }

        Blockchain::ledger.insert(nextBlock); 
        return true; 
    }
    catch(exception &ex)
    {  cout << "Could not append block to chain." << endl; return false; }
}

Block *Blockchain::getBlock(string hash)
{
    iter find = this->ledger.find(hash);

    if(find == this->ledger.end())
    {
       cout << "Could not retrieve block with hash " << hash << endl;
       return NULL;
    }

    return this->ledger.at(hash);
}

Block *Blockchain::mineBlockConcurrently(string data, string nodeAddress)
{
    Block *nextBlock = NULL;

    while(!nextBlock)
    {
        auto future = async([this, data, nodeAddress]{ return this->mineBlock(data, nodeAddress); });
        nextBlock = future.get(); 
    }

    return nextBlock;
}

long unsigned int countPoW(Ledger l)
{
    long unsigned int pow = 0;
    iter i;

    while(i != l.end()) pow += i->second->getDifficulty();

    return pow;
}

bool Blockchain::findConsensus(Blockchain *foreignChain)
{
    if(countPoW(foreignChain->getLedger()) > countPoW(this->ledger))
    {
        this->ledger = foreignChain->getLedger();
        return true;
    }

    return false;
}

long unsigned int Blockchain::getLedgerSize()
{ return (long unsigned int) this->ledger.size(); }

Block *Blockchain::getLastBlock()
{ return this->ledger.rbegin()->second; }

unsigned int Blockchain::getDifficultyLimit()
{ return this->difficultyLimit; }

long unsigned int Blockchain::getTryLimit()
{ return this->tryLimit; }

void Blockchain::setDifficultyLimit(unsigned int difficulyLimit)
{ this->difficultyLimit = difficultyLimit; }

void Blockchain::setDifficulty(long unsigned int difficulty)
{ this->difficulty = difficulty; }

Ledger Blockchain::getLedger()
{ return this->ledger; }

long unsigned int Blockchain::getDifficulty()
{ return this->difficulty; }

void Blockchain::setTryLimit(long unsigned int tryLimit)
{ this->tryLimit = tryLimit; }