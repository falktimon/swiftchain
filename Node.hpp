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

        std::string nodeName;
        std::string nodeAddress;

        std::map<std::string, std::string> neighborNodes;
        int tries;

    protected:
    public:

        Node(std::string nodeName);

        std::string getNodeName();
        std::string getNodeAddress();

        void setNodeName(std::string nodeName);
        void setNodeAddress(std::string nodeAddress);

        std::vector<std::string> readDataByRange(int range, Blockchain *chain);
        bool writeData(std::string data, Blockchain *chain, int tryLimit);
};

#endif