#include "../header/BitsetHash.h"
#include "../header/VectorState.h"
#include "../header/searches.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <vector>

// finds answer in depth 12
using namespace std;
int treeFromGoalStateLastCheckedNodeIndex = -1;
smallInt startTreeDepth = 0;
smallInt goalTreeDepth = 0;

typedef std::vector<smallInt> state_holder;

class Node {
  public:
    state_holder puzzleState;
    unsigned int parentNodeIndex;
    smallInt depth;
    long long int hash;
    Node(state_holder puzzleState1, int parentNode1, smallInt depth1, hash_holder hash1)
        : puzzleState(puzzleState1), parentNodeIndex(parentNode1), depth(depth1), hash(hash1) {}
};
std::unordered_map<hash_holder, int> fastLookUpGoal; // to store last two deepest layers of tree
std::string getLinesInReverseOrder(std::string *input) {
    std::string result;
    std::vector<std::string> lines;
    std::istringstream iss(*input);
    for (std::string line; std::getline(iss, line);) {
        lines.push_back(line);
    }
    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
        result.append(*it + "\n");
    }

    return result;
}
std::string makeSolutionString(vector<Node> &nodeList, int i, bool isExpandingFromGoal) {
    std::string result;
    Node current = nodeList[i];
    result.append(printState(current.puzzleState) + "\n");
    // cout << "Node list size : " << nodeList.size() << std::endl;
    do {
        current = nodeList[current.parentNodeIndex];
        result.append(printState(current.puzzleState) + "\n");

    } while (current.parentNodeIndex != -1);
    if (!isExpandingFromGoal) {
        return getLinesInReverseOrder(&result);
    }

    return result;
}

state_holder copyStateArrayForNewNode(state_holder firstStateVector, int swapIndex1, int swapIndex2) {
    state_holder newStateVector(firstStateVector);
    swap(newStateVector, swapIndex1, swapIndex2);
    return newStateVector;
}

void printTreeDepth(smallInt treeDepth) {
    cout << "----------------------------------------------------------------" << endl;
    cout << "Depth increased : " << static_cast<uint16_t>(startTreeDepth) << endl;
    cout << "----------------------------------------------------------------" << endl;
}

void addChildren(int puzzleSideSize, int i, vector<Node> &nodeList, bool isExpandingFromGoal) {
    int arraySize = puzzleSideSize * puzzleSideSize;

    int zeroIndex = getZeroIndex((nodeList)[i].puzzleState);
    if (zeroIndex >= puzzleSideSize) { // up
        state_holder newChildState =
            copyStateArrayForNewNode((nodeList)[i].puzzleState, zeroIndex, zeroIndex - puzzleSideSize);
        (nodeList).push_back(Node(newChildState, i, (nodeList)[i].depth + 1, makeHash(newChildState)));
        if (isExpandingFromGoal)
            fastLookUpGoal.insert({nodeList.back().hash, nodeList.size() - 1});
    }
    if (zeroIndex < (arraySize - puzzleSideSize)) { // down
        state_holder newChildState =
            copyStateArrayForNewNode((nodeList)[i].puzzleState, zeroIndex, zeroIndex + puzzleSideSize);
        (nodeList).push_back(Node(newChildState, i, (nodeList)[i].depth + 1, makeHash(newChildState)));
        if (isExpandingFromGoal)
            fastLookUpGoal.insert({nodeList.back().hash, nodeList.size() - 1});
    }
    if (zeroIndex % puzzleSideSize != 0) { // left
        state_holder newChildState =
            copyStateArrayForNewNode((nodeList)[i].puzzleState, zeroIndex, zeroIndex - 1);
        (nodeList).push_back(Node(newChildState, i, (nodeList)[i].depth + 1, makeHash(newChildState)));
        if (isExpandingFromGoal)
            fastLookUpGoal.insert({nodeList.back().hash, nodeList.size() - 1});
    }
    if (zeroIndex % puzzleSideSize != puzzleSideSize - 1) { // right
        state_holder newChildState =
            copyStateArrayForNewNode((nodeList)[i].puzzleState, zeroIndex, zeroIndex + 1);
        (nodeList).push_back(Node(newChildState, i, (nodeList)[i].depth + 1, makeHash(newChildState)));
        if (isExpandingFromGoal)
            fastLookUpGoal.insert({nodeList.back().hash, nodeList.size() - 1});
    }
}
void logState(vector<Node> &nodesList, int i, bool isStart) {
    cout << "-----------------------------" << endl;
    if (isStart) {
        cout << "Expanding tree from start state" << endl;
    } else {
        cout << "Expanding tree from goal state" << endl;
    }
    cout << "This is node : " << i << endl;
    cout << printState(nodesList[i].puzzleState) << endl;
    cout << "Node depth : " << static_cast<uint16_t>(nodesList[i].depth) << endl;
}

void expandTreeFromGoalState(int puzzleSideSize, vector<Node> &nodesFromGoalState) {
    int arraySize = puzzleSideSize * puzzleSideSize;
    int i = treeFromGoalStateLastCheckedNodeIndex + 1;
    goalTreeDepth++;
    for (; nodesFromGoalState[i].depth <= goalTreeDepth; i++) {

        if (i % 16384 == 0)
            logState(nodesFromGoalState, i, false);

        addChildren(puzzleSideSize, i, nodesFromGoalState, true);
    }

    treeFromGoalStateLastCheckedNodeIndex = i - 1;
}

void showBDSSteps(int startState[], int goalState[], int puzzleSideSize) {
    vector<Node> nodesFromStartState;
    vector<Node> nodesFromGoalState;
    int arraySize = puzzleSideSize * puzzleSideSize;
    vector<smallInt> goalStateVector;
    vector<smallInt> startStateVector;

    startStateVector.assign(startState, startState + arraySize);
    goalStateVector.assign(goalState, goalState + arraySize);

    nodesFromStartState.push_back(Node(startStateVector, -1, 0, makeHash(startStateVector)));
    nodesFromGoalState.push_back(Node(goalStateVector, -1, 0, makeHash(goalStateVector)));

    fastLookUpGoal.insert({nodesFromGoalState.back().hash, 0});
    for (int i = 0; i < nodesFromStartState.size(); ++i) {

        if (i % 16384 == 0)
            logState(nodesFromStartState, i, true);
        if (nodesFromStartState[i].depth > goalTreeDepth) {
            expandTreeFromGoalState(puzzleSideSize, nodesFromGoalState);
            startTreeDepth++;
        }

        if (fastLookUpGoal.find(nodesFromStartState[i].hash) != fastLookUpGoal.end()) {
            cout << "Found a mutual node " << endl;
            cout << makeSolutionString(nodesFromStartState, i, false);
            cout << makeSolutionString(nodesFromGoalState,
                                       (fastLookUpGoal.find(nodesFromStartState[i].hash)->second), true);

            exit(EXIT_SUCCESS);
        }

        addChildren(puzzleSideSize, i, nodesFromStartState, false);
    }
}
