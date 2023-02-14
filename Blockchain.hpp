#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include <map>
#include <string>

#ifndef BLOCK_HEADER
#define BLOCK_HEADER
#include "Block.hpp"
#endif

#include <stdint.h>
#include <ctime>
#include <thread>
#include <future>

typedef std::map<std::string, Block *> Ledger;

//! Blockchain
/*! This class implements a Blockchain structure and various methods
for interacting with the ledger.*/
class Blockchain {

    private:

        Ledger ledger;
        unsigned int difficulty;
        long unsigned int tryLimit;
        long unsigned int diffReductionTime;
        unsigned int difficultyLimit;

        bool appendBlock(Block *block);
        void adjustBlockIDAndAppend(Block *block);
        void adjustDifficulty();

    protected:
    public:
        Blockchain(long unsigned int tryLimit, 
                    unsigned int difficultyLimit, std::string userName, 
                    std::string genesisData, float diffReductionTime);

        bool verifyBlock(Block *block);
        Block *mineBlock(std::string data, std::string nodeAddress);
        Block *mineBlockConcurrently(std::string data, std::string nodeAddress);

        bool findConsensus(Blockchain *foreignChain);

        Block *getLastBlock();
        Ledger getLedger();
        Block *getBlock(std::string hash);
        unsigned int getDifficultyLimit();
        long unsigned int getTryLimit();
        long unsigned int getDifficulty();
        long unsigned int getLedgerSize();
        long unsigned int getDiffReductionTime();

        void setDifficultyLimit(unsigned int difficultyLimit);
        void setTryLimit(long unsigned int tryLimit);
        void setDifficulty(long unsigned int difficulty);
        void setDiffReductionTime(long unsigned int time);
};

#endif
