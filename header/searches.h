#ifndef SEARCHES_H
#define SEARCHES_H

void showDFSSteps(int startStateArray[], int goalStateArray[], int puzzleSideSize);
void showAstarSolution(int startStateArray[], int goalStateArray[], int puzzleSideSize , char* heuristic);
void showBDSSteps(int startState[], int goalState[], int puzzleSideSize);
void showIDAstarSolution(int startStateArray[], int goalStateArray[], int puzzleSideSize , char* heuristic);
void showBFSSteps(int startStateArray[], int goalStateArray[], int puzzleSideSize);
#endif