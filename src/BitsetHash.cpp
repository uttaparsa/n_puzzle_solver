#include "../header/BitsetHash.h"



 bitset_state convertToBitset(state_holder array, int arraySize) {
    std::bitset<64> result;
    for (int i = 0; i < arraySize; i++) {
        std::bitset<4> newBS(array[arraySize - 1 - i]);
        for (int j = 0; j < 4; j++) {
            result[i * 4 + j] = newBS[j];
        }
    }
    return result;
}

  hash_holder makeHash(state_holder &myNode) { return convertToBitset(myNode, myNode.size()).to_ullong(); }
