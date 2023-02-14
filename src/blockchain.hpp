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
for interacting with the ledger.*/
class Blockchain {

    private:

        Ledger ledger;
        unsigned int difficulty;
        long unsigned int try_limit;
        long unsigned int diff_redux_time;
        unsigned int difficulty_limit;

        bool append_block(Block *block);
        void adjust_block_id_and_append(Block *block);
        void adjust_difficulty();

    protected:
    public:
        Blockchain(long unsigned int try_limit, 
                    unsigned int difficulty_limit, std::string user_name, 
                    std::string genesis_data, float diff_redux_time);

        bool verify_block(Block *block);
        Block *mine_block(std::string data, std::string node_address);
        Block *mine_block_concurrently(std::string data, std::string node_address);

        bool find_consensus(Blockchain *foreign_chain);

        Block *get_last_block();
        Ledger get_ledger();
        Block *get_block(std::string hash);
        unsigned int get_difficulty_limit();
        long unsigned int get_try_limit();
        long unsigned int get_difficulty();
        long unsigned int get_ledger_size();
        long unsigned int get_diff_reduction_time();

        void set_difficulty_limit(unsigned int difficulty_limit);
        void set_try_limit(long unsigned int try_limit);
        void set_difficulty(long unsigned int difficulty);
        void set_diff_reduction_time(long unsigned int time);
};

#endif
