#ifndef HASH_HEADER
#define HASH_HEADER
#include "HashingUtil.hpp"
#endif

#ifndef BLOCKCHAIN_CPP
#define BLOCKCHAIN_CPP
#include "Blockchain.cpp"
#endif

#ifndef NODE_HEADER
#define NODE_HEADER
#include "Node.hpp"
#endif

#include <iostream>

using namespace std;

typedef reverse_iterator<_Rb_tree_iterator<pair<const string, Block *>>> rIter;

Node::Node(string nodeName)
{
    this->nodeName = nodeName;
    this->nodeAddress = generateSHAHash<string>(nodeName);
}

bool Node::writeData(string data, Blockchain *chain, int tryLimit = 10)
{
    if(data == "") return false;

    Block *block = chain->mineBlockConcurrently(data, this->nodeAddress);

    if(!block) 
    {
        if(this->tries++ == tryLimit) return false;
        writeData(data, chain, tryLimit);
    }

    this->tries = 0;
    return true;
}

vector<string> Node::readDataByRange(int range, Blockchain *chain)
{
    // Get a range of block data from the ledger
    Ledger ledger = chain->getLedger();
    rIter backwards = ledger.rbegin();

    vector<string> content;

    for(int i = 0; i < range; i++)
    {
        content.push_back(backwards->second->getData());
        backwards++;
    }

    return content;
}

string Node::getNodeAddress()
{ return this->nodeAddress; }

string Node::getNodeName()
{ return this->nodeName; }

void Node::setNodeAddress(string nodeAddress)
{ this->nodeAddress = nodeAddress; }

void Node::setNodeName(string nodeName)
{ this->nodeName = nodeName; }
