#ifndef VECTORSTATE_H
#define VECTORSTATE_H

#include <algorithm>
#include <iostream>
#include <vector>

typedef short smallInt;
typedef std::vector<smallInt> state_holder;
typedef unsigned long long int hash_holder;
// int puzzleSideSize;
std::string printState(state_holder& state);
int getZeroIndex(state_holder &state) ;
int getPuzzlePieceIndex(state_holder &state, smallInt target);
void swap(state_holder &ary, int index1, int index2);
std::vector<bool> getZeroBoolVector();
void printBoolVector(std::vector<bool> arr);

#endif