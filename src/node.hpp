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

#ifndef NODE_HPP
#define NODE_HPP

#ifndef BLOCKCHAIN_HEADER
#define BLOCKCHAIN_HEADER
#include "blockchain.hpp"
#endif

#include <string>
#include <map>

//! Node
/*! An object of a Node class represents a blockchain user, in the broadest sense. 
It may supply a base for a node in a P2P network or may represent a local user with access rights for a 
relevant blockchain.*/
class Node {

    private:

        std::string node_name;
        std::string node_address;

        int tries;

    protected:
    public:

        Node(std::string node_name);

        std::string get_node_name();
        std::string get_node_address();

        void set_node_name(std::string node_name);
        void set_node_address(std::string node_address);

        std::vector<std::string> read_data_by_range(unsigned int range, Blockchain *chain);
        std::vector<std::string> read_data_by_meta(std::string meta, Blockchain *chain);
        Block *get_block_by_index(unsigned int index, Blockchain *chain);
        std::vector<Block *> get_blocks_by_meta(std::string meta, Blockchain *chain);
        bool write_data(std::string data, Blockchain *chain, int try_limit, std::string meta_data);
};

#endif