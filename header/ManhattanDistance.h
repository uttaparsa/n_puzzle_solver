#ifndef MANHATTANDISTANCE_H_
#define  MANHATTANDISTANCE_H_

#include"VectorState.h"

int manhattandistance(int firstNumberIndex , int secondNumberIndex , int puzzleSideSize);
int calculateHnUsingManhattanDistance(state_holder& state , state_holder& goalState ,int puzzleSideSize);
int calculateFnUsingManhattanDistance(state_holder& state , state_holder& goalState ,smallInt stateGn,int puzzleSideSize);
int calculateLinear(state_holder state, int puzzleSideSize);
int calculateFnUsingManhattanDistanceAndLC(state_holder& state , state_holder& goalState ,smallInt stateGn,int puzzleSideSize);
#endif