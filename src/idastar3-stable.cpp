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
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// #include"../Header/DisjointDB.h"
// computes answer in depth 37 in less than a minute
// the one with vector of uints and visited children
// the difference between this and idastar 2 is that this code uses both manhattan distance and linear conflicts
using namespace std;
struct node_info {
    vector<smallInt> block;
    smallInt fn;
    smallInt gn;
    vector<bool> visitedChildren;
};
bool popDone = false;
typedef std::pair<hash_holder, node_info> node;
smallInt cutOff;
smallInt minFvalueFound;
typedef std::vector<node> NodeContainer;
typedef std::vector<smallInt> state_holder;
const int DOWN = 0;
const int UP = 1;
const int RIGHT = 2;
const int LEFT = 3;

node idaGoal;

bool isIdaAnswer(node &state) { return (state.first == idaGoal.first); }

void printAnswer(NodeContainer* parentsContainer) {
    for (int i = 0; i < (*parentsContainer).size(); i++) {
        cout << printState((*parentsContainer)[i].second.block) << endl;
    }
}
void showIDAstarSolution(int startStateArray[], int goalStateArray[], int puzzleSideSize , char* heuristic) {
    NodeContainer parentsContainer;
    int arraySize = puzzleSideSize * puzzleSideSize;
    state_holder startState(startStateArray, startStateArray + arraySize);
    state_holder goalState(goalStateArray, goalStateArray + arraySize);
    idaGoal = {makeHash(goalState), {goalState, 0, 0, getZeroBoolVector()}};
    cutOff = calculateHnUsingManhattanDistance(startState, goalState, puzzleSideSize);
    node initialNode = {makeHash(startState) // initialize starting node
                        ,
                        {startState,
                         calculateHnUsingManhattanDistance(startState, goalState, puzzleSideSize), 0,
                         getZeroBoolVector()}};
    parentsContainer.push_back(initialNode);
    unsigned int i = 0;
    while (cutOff < 200) {
        minFvalueFound = 200;
        parentsContainer.push_back(initialNode);
        while (!parentsContainer.empty()) {
            node &currentNode = parentsContainer.back();
            i++;
            if (isIdaAnswer(currentNode)) {
                cout << "Found the answer! " << endl;
                printAnswer(&parentsContainer);
                exit(EXIT_SUCCESS);
            }
            if (i % 1000000 == 0) {
                std::cout << "current block : " << endl << printState(currentNode.second.block) << endl;
                std::cout << "current gn : " << static_cast<int16_t>(currentNode.second.gn) << endl;
                std::cout << "current fn : " << static_cast<int16_t>(currentNode.second.fn) << endl;
                std::cout << "current cutOff : " << static_cast<int16_t>(cutOff) << endl;
                std::cout << "parents container size : " << parentsContainer.size() << endl;
                std::cout << "current iteration : " << i << endl;
                printBoolVector(currentNode.second.visitedChildren);
                std::cout << "---------------------------------------------------" << endl;
            }
            int zeroIndex = getZeroIndex(currentNode.second.block);
            if ((zeroIndex % puzzleSideSize) !=
                0) { // left-----------------------------------------------------------

                state_holder newChildStateHolder = currentNode.second.block;
                swap(newChildStateHolder, zeroIndex, zeroIndex - 1);
                smallInt leftFn = calculateFnUsingManhattanDistanceAndLC(
                    newChildStateHolder, idaGoal.second.block, currentNode.second.gn + 1, puzzleSideSize);
                hash_holder newNodeHash = makeHash(newChildStateHolder);
                if (leftFn <= cutOff && currentNode.second.visitedChildren[LEFT] == false) {
                    parentsContainer.back().second.visitedChildren[LEFT] = true;
                    parentsContainer.push_back(
                        {newNodeHash,
                         {newChildStateHolder, leftFn, currentNode.second.gn + 1, getZeroBoolVector()}});
                    popDone = false;

                    continue;
                } else if (!popDone) {
                    minFvalueFound = (leftFn < minFvalueFound) ? leftFn : minFvalueFound;
                }

            }
            if (zeroIndex >= puzzleSideSize) { // up-----------------------------------------------------

                state_holder newChildStateHolder = currentNode.second.block;
                swap(newChildStateHolder, zeroIndex, zeroIndex - puzzleSideSize);
                smallInt upFn = calculateFnUsingManhattanDistanceAndLC(
                    newChildStateHolder, idaGoal.second.block, currentNode.second.gn + 1, puzzleSideSize);

                hash_holder newNodeHash = makeHash(newChildStateHolder);
                if (upFn <= cutOff && currentNode.second.visitedChildren[UP] == false) {
                    parentsContainer.back().second.visitedChildren[UP] = true;
                    parentsContainer.push_back(
                        {newNodeHash,
                         {newChildStateHolder, upFn, currentNode.second.gn + 1, getZeroBoolVector()}});
                    popDone = false;

                    continue;

                } else if (!popDone) {
                    minFvalueFound = (upFn < minFvalueFound) ? upFn : minFvalueFound;
                }
            }
            if ((zeroIndex % puzzleSideSize) !=
                (puzzleSideSize - 1)) { // right-----------------------------

                state_holder newChildStateHolder = currentNode.second.block;
                swap(newChildStateHolder, zeroIndex, zeroIndex + 1);
                smallInt rightFn = calculateFnUsingManhattanDistanceAndLC(
                    newChildStateHolder, idaGoal.second.block, currentNode.second.gn + 1, puzzleSideSize);

                hash_holder newNodeHash = makeHash(newChildStateHolder);

                if (rightFn <= cutOff && currentNode.second.visitedChildren[RIGHT] == false) {
                    parentsContainer.back().second.visitedChildren[RIGHT] = true;
                    parentsContainer.push_back({newNodeHash,
                                                {newChildStateHolder, rightFn, currentNode.second.gn + 1,
                                                 getZeroBoolVector()}});

                    popDone = false;
                    continue;
                } else if (!popDone) {
                    minFvalueFound = (rightFn < minFvalueFound) ? rightFn : minFvalueFound;
                }

            }
            if (zeroIndex < (arraySize - puzzleSideSize)) { // down

                state_holder newChildStateHolder = currentNode.second.block;
                swap(newChildStateHolder, zeroIndex, zeroIndex + puzzleSideSize);
                smallInt downFn = calculateFnUsingManhattanDistanceAndLC(
                    newChildStateHolder, idaGoal.second.block, currentNode.second.gn + 1, puzzleSideSize);

                hash_holder newNodeHash = makeHash(newChildStateHolder);

                if (downFn <= cutOff && currentNode.second.visitedChildren[DOWN] == false) {
                    parentsContainer.back().second.visitedChildren[DOWN] = true;
                    parentsContainer.push_back(
                        {newNodeHash,
                         {newChildStateHolder, downFn, currentNode.second.gn + 1, getZeroBoolVector()}});
                    popDone = false;

                    continue;
                } else if (!popDone) {
                    minFvalueFound = (downFn < minFvalueFound) ? downFn : minFvalueFound;
                }
            }
            parentsContainer.pop_back();
            popDone = true;
        }
        cout << "\n\n\n\n\n\n\noverall:new min value found is : " << static_cast<int16_t>(minFvalueFound)
             << endl;
        if ((minFvalueFound != 200)) {
            cutOff = minFvalueFound;
        }

        // usleep(2000000);
    }
}
