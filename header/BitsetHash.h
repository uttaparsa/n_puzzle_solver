#ifndef BITSETHASH_H
#define BITSETHASH_H
#include "VectorState.h"
#include <bitset>
#include <iostream>
typedef std::bitset<64> bitset_state;
bitset_state convertToBitset(state_holder array, int arraySize);
hash_holder makeHash(state_holder &myNode);

#endif