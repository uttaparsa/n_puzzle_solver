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
#include <queue>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
//the one with hash function
// can calculate up to depth 22
//bitset state

using namespace std;
typedef unsigned long long int hash_holder;
struct node_info {
    hash_holder hash;
    hash_holder parentHash;
};

typedef std::pair<hash_holder, hash_holder> ClosedNode;
typedef std::unordered_map<hash_holder, hash_holder> CLOSE;

typedef std::pair<state_holder, node_info> OpenNode;
typedef std::queue<OpenNode> OPEN;

hash_holder goal_hash;

OPEN openNodes;
extern CLOSE closedNodes;

unsigned long getNumberAtIndex(bitset_state state, smallInt index) {
    bitset<4> resultsubset;
    for (int i = 0; i < 4; ++i) {
        resultsubset[i] = state[index * 4 + i];
    }
    return resultsubset.to_ulong();
}

state_holder covertToArrayState(bitset_state bitsetState , int arraySize) {
    state_holder result;
    for (int i = 0; i < arraySize; ++i) {
        result.push_back(getNumberAtIndex(bitsetState, arraySize - i - 1 ));
    }
    return result;
}
bool isBFSAnswer(hash_holder &state) { return (state == goal_hash); }

bitset_state hash_to_state(hash_holder &state, int arraySize) {
    unsigned long long int num = state;
    bitset_state result(num);
    return result;
}

void printAns(hash_holder currentHash){
    hash_holder current = currentHash;
    while(current!=0){
        state_holder new_state = covertToArrayState(
            hash_to_state(current , 16) , 16); // TODO : remove these hardcodes
        cout << printState(new_state) << std::endl;
        
        CLOSE::iterator  currentFind = closedNodes.find(current);
        if(currentFind != closedNodes.end())
            current = currentFind->second;
        else break;
    }
}

void saveNode(state_holder &newNodeStateHolder,
              OpenNode &parent) { // only adds if meets A star conditions
    hash_holder newNodeHash = makeHash(newNodeStateHolder);
    if (closedNodes.find(newNodeHash) == closedNodes.end()) {
        openNodes.push({newNodeStateHolder, {newNodeHash, parent.second.hash}});
    }
}

void showBFSSteps(int startStateArray[], int goalStateArray[], int puzzleSideSize) {

    int arraySize = puzzleSideSize * puzzleSideSize;
    state_holder startState(startStateArray, startStateArray + arraySize);
    state_holder goalState(goalStateArray, goalStateArray + arraySize);
    goal_hash = makeHash(goalState);

    OpenNode currentNode = {startState, {makeHash(startState), 0}};
    openNodes.push(currentNode);

    unsigned int i = 0;
    while (!openNodes.empty()) {
        currentNode = openNodes.front();
        closedNodes.insert({currentNode.second.hash, currentNode.second.parentHash});
        if (isBFSAnswer(currentNode.second.hash)) {
            cout << "Found the answer! " << endl;
            printAns(currentNode.second.hash);
            exit(EXIT_FAILURE);
        }
       

        if (i % 200000 == 0) {
            std::cout << "current block : " << endl << printState(currentNode.first) << endl;
            std::cout << "open size : " << openNodes.size() << endl;
            std::cout << "closed size : " << closedNodes.size() << endl;
            std::cout << "current parent : " << (currentNode.second.parentHash) << endl;
            std::cout << "current hash : " << (currentNode.second.hash) << endl;
            std::cout << "current iteration : " << i << endl;
            std::cout << "---------------------------------------------------" << endl;
            // if (i > 10)
            //     break;
        }

        int zeroIndex = getZeroIndex(currentNode.first);
        if ((zeroIndex % puzzleSideSize) != 0) { // left

            state_holder newChildStateHolder = currentNode.first;
            swap(newChildStateHolder, zeroIndex, zeroIndex - 1);
            saveNode(newChildStateHolder, currentNode);
        }
        if (zeroIndex >= puzzleSideSize) { // up

            state_holder newChildStateHolder = currentNode.first;
            swap(newChildStateHolder, zeroIndex, zeroIndex - puzzleSideSize);
            saveNode(newChildStateHolder, currentNode);
        }
        if ((zeroIndex % puzzleSideSize) != (puzzleSideSize - 1)) { // right

            state_holder newChildStateHolder = currentNode.first;
            swap(newChildStateHolder, zeroIndex, zeroIndex + 1);
            saveNode(newChildStateHolder, currentNode);
        }
        if (zeroIndex < (arraySize - puzzleSideSize)) { // down

            state_holder newChildStateHolder = currentNode.first;
            swap(newChildStateHolder, zeroIndex, zeroIndex + puzzleSideSize);
            saveNode(newChildStateHolder, currentNode);
        }
        openNodes.pop();
        i++;
    }
}
