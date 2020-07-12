#include "../header/BitsetHash.h"
#include "../header/VectorState.h"
#include "../header/ManhattanDistance.h"
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
// computes answer in depth 40 in less than a minute
// the one with vector of short with new hash function
//using linear conflicts
//and using header files
using namespace std;
struct node_info {
    vector<smallInt> block;
    hash_holder parentHash;
    smallInt fn;
    smallInt gn;
};

typedef std::pair<hash_holder, node_info> node;

typedef std::set<std::pair<smallInt, hash_holder>>
    Ordered_Container; // opened nodes are also added to ordered container
typedef std::unordered_map<hash_holder, node_info> OPEN;
typedef std::unordered_map<hash_holder, node_info> CLOSE;
typedef std::vector<smallInt> state_holder;

OPEN openedNodes;
CLOSE closedNodes;
Ordered_Container openNodesSortedByF;
node goal;

bool isAstarAnswer(node &state) { return (state.first == goal.first); }

void printAnswer(node &current) {
    int i = 0;
    cout << printState(current.second.block) << endl;
    do {
        i++;
        current = {closedNodes.find(current.second.parentHash)->first,
                   closedNodes.find(current.second.parentHash)->second};
        cout << printState(current.second.block) << endl;

    } while (current.second.parentHash != 0);
}

void saveNode(smallInt fn, state_holder &newNodeStateHolder,
              node &parent) { // only adds if meets A star conditions
    hash_holder newNodeHash = makeHash(newNodeStateHolder);
    if (closedNodes.find(newNodeHash) == closedNodes.end()) {
        if (openedNodes.find(newNodeHash) == closedNodes.end()) {
            openNodesSortedByF.insert({fn, newNodeHash});
            openedNodes.insert(
                {newNodeHash, {newNodeStateHolder, parent.first, fn, parent.second.gn + 1}});
        } else {
            auto existing = openedNodes.find(newNodeHash);
            if (existing->second.gn > parent.second.gn + 1) {
                openNodesSortedByF.erase({existing->second.fn, existing->first});
                openNodesSortedByF.insert({fn, newNodeHash});
                openedNodes.erase(newNodeHash);
                openedNodes.insert(
                    {newNodeHash, {newNodeStateHolder, parent.first, fn, parent.second.gn + 1}});
            }
        }
    }
}

void showAstarSolution(int startStateArray[], int goalStateArray[], int puzzleSideSize , char* heuristic) {

    int arraySize = puzzleSideSize * puzzleSideSize;
    state_holder startState(startStateArray, startStateArray + arraySize);
    state_holder goalState(goalStateArray, goalStateArray + arraySize);
    goal = {makeHash(goalState), {goalState, 0, 0, 0}};

    node currentNode = {makeHash(startState) // initialize starting node
                        ,
                        {startState, 0, 0, 0}};
    openedNodes.insert(currentNode);
    openNodesSortedByF.insert({currentNode.second.fn, currentNode.first});

    unsigned int i = 0;
    while (!openedNodes.empty()) {
        currentNode = {openedNodes.find(openNodesSortedByF.begin()->second)->first,
                       openedNodes.find(openNodesSortedByF.begin()->second)->second};

        if (isAstarAnswer(currentNode)) {
            cout << "Found the answer! " << endl;
            printAnswer(currentNode);
            exit(EXIT_FAILURE);
        }
        if (i % 200000 == 0) {
            std::cout << "current block : " << endl << printState(currentNode.second.block) << endl;
            std::cout << "open size : " << openedNodes.size() << endl;
            std::cout << "closed size : " << closedNodes.size() << endl;
            std::cout << "current gn : " << currentNode.second.gn << endl;
            std::cout << "current fn : " << currentNode.second.fn << endl;
            std::cout << "current parent : " << (currentNode.second.parentHash) << endl;
            std::cout << "current iteration : " << i << endl;
            std::cout << "---------------------------------------------------" << endl;
            // if(i>1000) break;
        }

        int zeroIndex = getZeroIndex(currentNode.second.block);
        if ((zeroIndex % puzzleSideSize) != 0) { // left

            state_holder newChildStateHolder = currentNode.second.block;
            swap(newChildStateHolder, zeroIndex, zeroIndex - 1);

            int leftFn = calculateFnUsingManhattanDistanceAndLC(
                newChildStateHolder, goal.second.block, currentNode.second.gn + 1, puzzleSideSize);
            saveNode(leftFn, newChildStateHolder, currentNode);
        }
        if (zeroIndex >= puzzleSideSize) { // up

            state_holder newChildStateHolder = currentNode.second.block;
            swap(newChildStateHolder, zeroIndex, zeroIndex - puzzleSideSize);

            int upFn = calculateFnUsingManhattanDistanceAndLC(newChildStateHolder, goal.second.block,
                                                              currentNode.second.gn + 1, puzzleSideSize);
            saveNode(upFn, newChildStateHolder, currentNode);
        }
        if ((zeroIndex % puzzleSideSize) != (puzzleSideSize - 1)) { // right

            state_holder newChildStateHolder = currentNode.second.block;
            swap(newChildStateHolder, zeroIndex, zeroIndex + 1);

            int rightFn = calculateFnUsingManhattanDistanceAndLC(
                newChildStateHolder, goal.second.block, currentNode.second.gn + 1, puzzleSideSize);
            saveNode(rightFn, newChildStateHolder, currentNode);
        }
        if (zeroIndex < (arraySize - puzzleSideSize)) { // down

            state_holder newChildStateHolder = currentNode.second.block;
            swap(newChildStateHolder, zeroIndex, zeroIndex + puzzleSideSize);

            int downFn = calculateFnUsingManhattanDistanceAndLC(
                newChildStateHolder, goal.second.block, currentNode.second.gn + 1, puzzleSideSize);
            saveNode(downFn, newChildStateHolder, currentNode);
        }
        closedNodes.insert(currentNode);
        openedNodes.erase(currentNode.first);
        openNodesSortedByF.erase({currentNode.second.fn, currentNode.first});
        i++;
    }
}
