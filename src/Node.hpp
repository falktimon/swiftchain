#ifndef NODE_HPP
#define NODE_HPP

#ifndef BLOCKCHAIN_HEADER
#define BLOCKCHAIN_HEADER
#include "Blockchain.hpp"
#endif

#include <string>
#include <map>

class Node {

    private:

        std::string node_name;
        std::string node_address;

        std::map<std::string, std::string> neighborNodes;
        int tries;

    protected:
    public:

        Node(std::string node_name);

        std::string get_node_name();
        std::string get_node_address();

        void set_node_name(std::string node_name);
        void set_node_address(std::string node_address);

        std::vector<std::string> read_data_by_range(int range, Blockchain *chain);
        bool write_data(std::string data, Blockchain *chain, int try_limit);
};

#endif