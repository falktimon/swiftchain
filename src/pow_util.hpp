#ifndef POWUTIL_HPP
#define POWUTIL_HPP

#include "block.hpp"

#include <string>

/*  Simple hashcash implementation. */

//! find_trailing_zero(string)
/*! Parameters: A SHA hash to be examined for trailing zeros
This function finds the number of trailing zeros in any given SHA hash.*/
int find_trailing_zeros(std::string hash)
{
    unsigned short int i = 0;

    // Count the number of uninterrupted zeros in the given hash:
    while((hash[i++] == '0') && (i != (unsigned int)hash.size() - 1));

    return i;
}

const std::string hex_to_bin(char c)
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

//! convert_to_bin(string)
/*! Parameters: A SHA hash in hexadecimal notation.
This function converts a hash in hexadecimal notation into binary 
notation and returns it as a string.*/
std::string convert_to_bin(std::string hash)
{
    std::string binary;

    // Assemble binary string:
    for(int i = 0; i < (int) hash.size(); i++)
        binary += hex_to_bin(hash[i]);

    return binary;
}

//! run_hash_cash(Block *)
/*! Parameters: A Block object.
Runs the hashcash proof-of-work algorithm on a given block.
Returns true if the block hash satisfies the difficulty contained within the block,
else it returns false.*/
bool run_hash_cash(Block *block)
{
    std::string hash = convert_to_bin(block->get_block_hash());
    long unsigned int difficulty = block->get_difficulty();

    unsigned short int zeros;

    // Check if block hash satisfies the current difficulty:
    if((zeros = find_trailing_zeros(hash)) >= difficulty)
        return true;

    return false;
}

//! verify_attempt(Block *)
/*! Parameters: A block object.
This function tries to verify a block against a proof-of-work algorithm.
Currently only hashcash is supported.*/
bool verify_attempt(Block *block)
{
   return run_hash_cash(block);
}

#endif
