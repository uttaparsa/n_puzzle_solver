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
#include <unordered_map>
#include <vector>

// the one with vector of short with new hash function
using namespace std;
struct node_info {
    vector<smallInt> block;
    hash_holder parentHash;
    smallInt gn;
};

typedef std::pair<hash_holder, node_info> node;

typedef std::set<std::pair<smallInt, hash_holder>> Ordered_Container; // opened nodes are also added to ordered container
typedef std::unordered_map<hash_holder, node_info> OPEN;
typedef std::unordered_map<hash_holder, node_info> CLOSE;

const smallInt UP_COST = 1;
const smallInt DOWN_COST = 2;
const smallInt RIGHT_COST = 3;
const smallInt LEFT_COST = 4;

OPEN openedNodes;
CLOSE closedNodes;
Ordered_Container openNodesSortedByG;
node goal;

bool isAnswer(node &state) { return (state.first == goal.first); }
bitset<64> convertToBitset(state_holder array, int arraySize) {
    bitset<64> result;
    for (int i = 0; i < arraySize; i++) {
        bitset<4> newBS(array[arraySize - 1 - i]);
        for (int j = 0; j < 4; j++) {
            result[i * 4 + j] = newBS[j];
        }
    }
    return result;
}

void printAnswer(node &current) {
    int i = 0;
    cout << printState(current.second.block) << endl;
    do {
        i++;
        current = {closedNodes.find(current.second.parentHash)->first,
                   closedNodes.find(current.second.parentHash)->second};
        cout << i << " : " << printState(current.second.block) << endl;

    } while (current.second.parentHash != 0);
}

void saveNode(short gn, state_holder &newNodeStateHolder,
              node &parent) { // only adds if meets A star conditions
    hash_holder newNodeHash = makeHash(newNodeStateHolder);
    if (closedNodes.find(newNodeHash) == closedNodes.end()) {
        if (openedNodes.find(newNodeHash) == closedNodes.end()) {
            openNodesSortedByG.insert({gn, newNodeHash});
            openedNodes.insert({newNodeHash, {newNodeStateHolder, parent.first, gn}});
        } else {
            auto existing = openedNodes.find(newNodeHash);
            if (existing->second.gn > parent.second.gn + 1) {
                openNodesSortedByG.erase({existing->second.gn, existing->first});
                openNodesSortedByG.insert({gn, newNodeHash});
                openedNodes.erase(newNodeHash);
                openedNodes.insert({newNodeHash, {newNodeStateHolder, parent.first, gn}});
            }
        }
    }
}

void showUCSSteps(int startStateArray[], int goalStateArray[], int puzzleSideSize) {

    int arraySize = puzzleSideSize * puzzleSideSize;
    state_holder startState(startStateArray, startStateArray + arraySize);
    state_holder goalState(goalStateArray, goalStateArray + arraySize);
    goal = {makeHash(goalState), {goalState, 0, 0}};

    node currentNode = {makeHash(startState) // initialize starting node
                        ,
                        {startState, 0, 0}};
    openedNodes.insert(currentNode);
    openNodesSortedByG.insert({currentNode.second.gn, currentNode.first});

    unsigned int i = 0;
    while (!openedNodes.empty()) {
        currentNode = {openedNodes.find(openNodesSortedByG.begin()->second)->first,
                       openedNodes.find(openNodesSortedByG.begin()->second)->second};

        if (isAnswer(currentNode)) {
            cout << "Found the answer! " << endl;
            // closedNodes.insert(currentNode);
            printAnswer(currentNode);
            exit(EXIT_FAILURE);
        }
        if (i % 200000 == 0) {
            std::cout << "current block : " << endl << printState(currentNode.second.block) << endl;
            std::cout << "open size : " << openedNodes.size() << endl;
            std::cout << "closed size : " << closedNodes.size() << endl;
            std::cout << "current gn : " << currentNode.second.gn << endl;
            std::cout << "current parent : " << (currentNode.second.parentHash) << endl;
            std::cout << "current iteration : " << i << endl;
            std::cout << "---------------------------------------------------" << endl;
            // if(i>1000) break;
        }

        int zeroIndex = getZeroIndex(currentNode.second.block);
        if ((zeroIndex % puzzleSideSize) != 0) { // left

            state_holder newChildStateHolder = currentNode.second.block;
            swap(newChildStateHolder, zeroIndex, zeroIndex - 1);

            int leftGn = currentNode.second.gn + LEFT_COST;
            saveNode(leftGn, newChildStateHolder, currentNode);
        }
        if (zeroIndex >= puzzleSideSize) { // up

            state_holder newChildStateHolder = currentNode.second.block;
            swap(newChildStateHolder, zeroIndex, zeroIndex - puzzleSideSize);

            int upGn = currentNode.second.gn + UP_COST;
            saveNode(upGn, newChildStateHolder, currentNode);
        }
        if ((zeroIndex % puzzleSideSize) != (puzzleSideSize - 1)) { // right

            state_holder newChildStateHolder = currentNode.second.block;
            swap(newChildStateHolder, zeroIndex, zeroIndex + 1);

            int rightGn = currentNode.second.gn + RIGHT_COST;
            saveNode(rightGn, newChildStateHolder, currentNode);
        }
        if (zeroIndex < (arraySize - puzzleSideSize)) { // down

            state_holder newChildStateHolder = currentNode.second.block;
            swap(newChildStateHolder, zeroIndex, zeroIndex + puzzleSideSize);

            int downGn = currentNode.second.gn + DOWN_COST;
            saveNode(downGn, newChildStateHolder, currentNode);
        }
        closedNodes.insert(currentNode);
        openedNodes.erase(currentNode.first);
        openNodesSortedByG.erase({currentNode.second.gn, currentNode.first});
        i++;
    }
}
