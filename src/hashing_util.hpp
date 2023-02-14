#include <cstdlib>
#include <iostream>
#include <vector>
#include "PicoSHA2/picosha2.h"

using namespace std;

std::string generate_sha_hash(string data)
{
    vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(data.begin(), data.end(),
                        hash.begin(), hash.end());

    return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}
