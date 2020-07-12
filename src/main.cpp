#include<iostream>
#include<stdio.h>
#include<string>
#include<algorithm>
#include "../header/searches.h"
#include "../header/VectorState.h"


using namespace std;

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

enum string_code {
    astar,
    idastar,
    DFS,
    BFS,
    UCS,
    BDS,
    NONE
};
string_code hashit (std::string const& inString) {
    if (inString == "astar") return astar;
    if (inString == "idastar") return idastar;
    if (inString == "dfs") return DFS;
    if (inString == "bfs") return BFS;
    if (inString == "bds") return BDS;
    return NONE;
}
int main(int argc, char **argv) {
    FILE *fp;
    cout << "main started "<< endl;

    if(argc < 2){
        std::cout << "please enter input file name!";
        return 0;
    }
    if(!cmdOptionExists(argv, argv+argc, "-m")){
        std::cout << "use -m to select search method";
        return 0;
    }

    char* inputName =  argv[1];
    fp = fopen(inputName, "r");
    char * searchMethod = getCmdOption(argv, argv + argc, "-m");
    char * heuristic = getCmdOption(argv, argv + argc, "-h");

    
    int puzzleSideSize;

    fscanf(fp, "%d ", &puzzleSideSize);
    cout << "side size is : " << puzzleSideSize << endl;

    int arraySize = puzzleSideSize * puzzleSideSize;

    int i = 0;

    int *startStateNumbers = new int[arraySize]; 
    for (int i = 0; i < arraySize; i++) {
        fscanf(fp, "%d, ", &startStateNumbers[i]);
    }
    cout << endl;

    int *endStateNumbers = new int[arraySize];
    for (int i = 0; i < arraySize; i++) {
        fscanf(fp, "%d, ", &endStateNumbers[i]);
    }

    switch (hashit(searchMethod))
    {
    case astar:
        showAstarSolution(startStateNumbers, endStateNumbers, puzzleSideSize , heuristic);
        break;
    case DFS:
        showDFSSteps(startStateNumbers, endStateNumbers, puzzleSideSize);
        break;
    case BDS:
        showBDSSteps(startStateNumbers, endStateNumbers, puzzleSideSize);
        break;
    case idastar:
        showIDAstarSolution(startStateNumbers, endStateNumbers, puzzleSideSize , heuristic);
        break;
    case BFS:
        showBFSSteps(startStateNumbers, endStateNumbers, puzzleSideSize);
        break;
    default:
        std::cout << "Search method not found" << endl;
        break;
    }
    

    fclose(fp);
}