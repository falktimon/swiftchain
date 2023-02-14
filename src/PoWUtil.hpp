#ifndef POWUTIL_HPP
#define POWUTIL_HPP

#include "Block.hpp"

#include <string>

/*  Simple hashcash implementation. */

int findTrailingZeros(std::string hash)
{
    unsigned short int i = 0;

    while((hash[i++] == '0') && (i != (unsigned int)hash.size() - 1));

    return i;
}

const std::string hexCharToBin(char c)
{
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        default: return "0";
    }
}

std::string convertToBinary(std::string hash)
{
    std::string binary;

    for(int i = 0; i < (int) hash.size(); i++)
        binary += hexCharToBin(hash[i]);

    return binary;
}

bool runHashCash(Block *block)
{
    std::string hash = convertToBinary(block->getBlockHash());
    long unsigned int difficulty = block->getDifficulty();

    unsigned short int zeros;

    if((zeros = findTrailingZeros(hash)) >= difficulty)
        return true;

    return false;
}

bool verifyAttempt(Block *block)
{
   return runHashCash(block);
}

#endif
