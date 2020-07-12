#include "../header/BitsetHash.h"
#include "../header/VectorState.h"
#include "../header/searches.h"
#include <algorithm>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// the one with vector of short
//iterative dfs
using namespace std;
struct node {
    vector<smallInt> block;
    vector<bool> visitedChildren;
    hash_holder hash;
};
const unsigned int MAX_DEPTH = 18;
typedef std::unordered_set<hash_holder> MapContainer;
typedef std::vector<node> NodeContainer;

NodeContainer parentsContainer;
MapContainer fastParentsContainer;

hash_holder goalHash;
const int DOWN = 0;
const int UP = 1;
const int RIGHT = 2;
const int LEFT = 3;


bool isAnswer(hash_holder &stateHash) { return (stateHash == goalHash); }

void printAnswer() {
    for (int i = 0; i < parentsContainer.size(); i++) {
        cout << printState(parentsContainer[i].block) << endl;
    }
}

void showDFSSteps(int startStateArray[], int goalStateArray[], int puzzleSideSize) {

    int arraySize = puzzleSideSize * puzzleSideSize;
    state_holder startState(startStateArray, startStateArray + arraySize);
    state_holder goalState(goalStateArray, goalStateArray + arraySize);
    goalHash = makeHash(goalState);

    parentsContainer.push_back({startState, getZeroBoolVector(), makeHash(startState)});
    fastParentsContainer.insert(makeHash(startState));
    unsigned long int i = 0;
    int currentDepth = 0;
    while (!parentsContainer.empty()) {
        node &current = parentsContainer.back();
        i++;

        if (i % 1000000 == 0) {
        std::cout << "current block : " << endl << printState(current.block) << endl;
        std::cout << "parents container size : " << parentsContainer.size() << endl;
        std::cout << "current iteration : " << i << endl;
        printBoolVector(current.visitedChildren);
        std::cout << "---------------------------------------------------" << endl;
        }
        if (makeHash(current.block) == goalHash) {
            cout << "Found the answer! " << endl;
            printAnswer();
            exit(EXIT_SUCCESS);
        } else if (currentDepth < MAX_DEPTH) {
            int zeroIndex = getZeroIndex(current.block);
            if ((zeroIndex % puzzleSideSize) != 0 && current.visitedChildren[LEFT] == false) { // left

                state_holder newChildStateHolder = current.block;
                swap(newChildStateHolder, zeroIndex, zeroIndex - 1);
                hash_holder newNodeHash = makeHash(newChildStateHolder);
                if (fastParentsContainer.find(newNodeHash) == fastParentsContainer.end()) {
                    parentsContainer.back().visitedChildren[LEFT] = true;
                    parentsContainer.push_back({newChildStateHolder, getZeroBoolVector(), newNodeHash});
                    fastParentsContainer.insert(newNodeHash);
                    currentDepth++;
                    continue;
                }
            }
            if (zeroIndex >= puzzleSideSize && current.visitedChildren[UP] == false) { // up

                state_holder newChildStateHolder = current.block;
                swap(newChildStateHolder, zeroIndex, zeroIndex - puzzleSideSize);
                hash_holder newNodeHash = makeHash(newChildStateHolder);
                if (fastParentsContainer.find(newNodeHash) == fastParentsContainer.end()) {
                    parentsContainer.back().visitedChildren[UP] = true;
                    parentsContainer.push_back({newChildStateHolder, getZeroBoolVector(), newNodeHash});
                    fastParentsContainer.insert(newNodeHash);
                    currentDepth++;
                    continue;
                }
            }
            if ((zeroIndex % puzzleSideSize) !=
                (puzzleSideSize - 1 ) && current.visitedChildren[RIGHT] == false) { // right

                state_holder newChildStateHolder = current.block;
                swap(newChildStateHolder, zeroIndex, zeroIndex + 1);
                hash_holder newNodeHash = makeHash(newChildStateHolder);
                if (fastParentsContainer.find(newNodeHash) == fastParentsContainer.end()) {
                    parentsContainer.back().visitedChildren[RIGHT] = true;
                    parentsContainer.push_back({newChildStateHolder, getZeroBoolVector(), newNodeHash});
                    fastParentsContainer.insert(newNodeHash);
                    currentDepth++;
                    continue;
                }
            }
            if (zeroIndex < (arraySize - puzzleSideSize) &&
                current.visitedChildren[DOWN] == false) { // down

                state_holder newChildStateHolder = current.block;
                swap(newChildStateHolder, zeroIndex, zeroIndex + puzzleSideSize);
                hash_holder newNodeHash = makeHash(newChildStateHolder);
                if (fastParentsContainer.find(newNodeHash) == fastParentsContainer.end()) {
                    parentsContainer.back().visitedChildren[DOWN] = true;
                    parentsContainer.push_back({newChildStateHolder, getZeroBoolVector(), newNodeHash});
                    fastParentsContainer.insert(newNodeHash);
                    currentDepth++;
                    continue;
                }
            }
        }
        fastParentsContainer.erase(parentsContainer.back().hash);
        parentsContainer.pop_back();
        currentDepth--;
    }
}
