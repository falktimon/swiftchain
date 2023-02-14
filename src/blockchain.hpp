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

#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include <map>
#include <string>

#ifndef BLOCK_HEADER
#define BLOCK_HEADER
#include "block.hpp"
#endif

#include <stdint.h>
#include <ctime>
#include <thread>
#include <future>

typedef std::map<std::string, Block *> Ledger;

//! Blockchain
/*! This class implements a Blockchain structure and various methods
for interacting with the ledger. The Node class exposes some higher-level wrappers
over some methods in this class.*/
class Blockchain {

    private:

        Ledger ledger;
        std::string blockchain_id;

        unsigned int difficulty;
        long unsigned int try_limit;
        long unsigned int diff_redux_time;
        unsigned int difficulty_limit;

        bool append_block(Block *block);
        void adjust_block_id_and_append(Block *block);
        void adjust_difficulty();

    protected:
    public:

        //! Blockchain constructor
        Blockchain(long unsigned int try_limit, 
                    unsigned int difficulty_limit, std::string node_addr, 
                    std::string genesis_data, float diff_redux_time);

        bool verify_block(Block *block);
        Block *mine_block(std::string data, std::string node_address, std::string data_description);
        Block *mine_block_concurrently(std::string data, std::string node_address, std::string meta_data);

        bool find_consensus(Blockchain *foreign_chain);

        std::vector<Block *> get_blocks_by_range(unsigned int range);
        Block *get_last_block();
        Ledger get_ledger();
        Block *get_block(std::string hash);
        unsigned int get_difficulty_limit();
        long unsigned int get_try_limit();
        long unsigned int get_difficulty();
        long unsigned int get_ledger_size();
        long unsigned int get_redux_time();
        std::string get_blockchain_id();

        void set_difficulty_limit(unsigned int difficulty_limit);
        void set_try_limit(long unsigned int try_limit);
        void set_difficulty(long unsigned int difficulty);
        void set_redux_time(float time);
};

#endif
