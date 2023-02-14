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
        bool write_data(std::string data, Blockchain *chain, int try_limit);
};

#endif