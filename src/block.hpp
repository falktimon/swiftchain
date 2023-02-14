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
        std::string meta_data;

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
              std::string blockchain_id, std::string meta_data);

        void set_node_address(std::string node_address);
        void set_block_id(long unsigned int block_id);
        void set_blockchain_id(std::string blockchain_id);
        void set_data(std::string data);
        void set_meta_data(std::string meta_data);

        std::string get_block_hash();
        std::string get_prev_hash();
        long int get_nonce();
        long unsigned int get_block_id();
        long unsigned int get_difficulty();
        std::string get_data();
        std::string get_node_address();   
        std::string get_blockchain_id();
        std::string get_timestamp();
        std::string get_meta_data();

};

#endif
