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
        long unsigned int block_id;
        long int nonce;
        long unsigned int difficulty;

        // Block Metadata
        std::string node_address;
        std::string timestamp;

        std::string data;

        // Relevant Hashes:
        std::string block_hash;
        std::string prev_hash;

        std::string blockchain_id;

        std::string generate_block_hash(Block *b);
        std::string generate_timestamp();

    protected:
    public:

        Block(std::string data, std::string user_address);
        Block(Block *prev, std::string data, std::string user_address, 
              long unsigned int nonce, long unsigned int difficulty,
              std::string blockchain_id);

        void set_node_address(std::string node_address);
        void set_block_id(long unsigned int block_id);
        void set_blockchain_id(std::string blockchain_id);
        void set_data(std::string data);

        std::string get_block_hash();
        std::string get_prev_hash();
        long int get_nonce();
        long unsigned int get_block_id();
        long unsigned int get_difficulty();
        std::string get_data();
        std::string get_node_address();   
        std::string get_blockchain_id();
        std::string get_timestamp();

};

#endif
