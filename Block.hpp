#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <cstdlib>
#include <chrono>

#include "./PicoSHA2/picosha2.h"

//! Block
/*! This class implements a block structure which may be mined using the mineBlock function 
from the Blockchain class. It provides two constructors, one Genesis constructor and 
and one common constructor.*/
class Block {

    /* Block superclass. */

    private:

        // Primitive Block Attributes:
        long unsigned int blockID;
        long int nonce;
        long unsigned int difficulty;

        // Block Metadata
        std::string nodeAddress;
        std::string timestamp;

        std::string data;

        // Relevant Hashes:
        std::string blockHash;
        std::string prevHash;

        std::string generateTimestamp();
        std::string generateBlockHash(Block *b);

    protected:
    public:

        Block(std::string data, std::string userAddress);
        Block(Block *prev, std::string data, std::string userAddress, 
              long unsigned int nonce, long unsigned int difficulty);

        void setContent(std::string data);
        std::string getContent();

        void setUserID(std::string userID);
        void setBlockID(long unsigned int blockID);
        void incrementNonce();

        std::string getBlockHash();
        std::string getPrevHash();
        long int getNonce();
        long unsigned int getBlockID();
        long unsigned int getDifficulty();
        std::string getData();
        std::string getNodeAddress();   
        std::string getTimestamp();

};

#endif
