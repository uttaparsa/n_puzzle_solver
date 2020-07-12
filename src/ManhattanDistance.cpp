#include "../header/ManhattanDistance.h"


int manhattandistance(int firstNumberIndex , int secondNumberIndex , int puzzleSideSize){ // manhattan distance

    int firstX = firstNumberIndex%puzzleSideSize;
    int firstY = firstNumberIndex/puzzleSideSize;
    int secondX = secondNumberIndex%puzzleSideSize;
    int secondY = secondNumberIndex/ puzzleSideSize;
    return abs(firstX - secondX) + abs(firstY - secondY);

}
int calculateHnUsingManhattanDistance(state_holder& state , state_holder& goalState ,int puzzleSideSize){
    int hn = 0;
    for(int i = 0 ; i < state.size() ; i++){
        int statePieceIndex = getPuzzlePieceIndex(state ,i);
        int goalStatePieceIndex = getPuzzlePieceIndex(goalState , i);
        hn += manhattandistance(statePieceIndex
                                ,goalStatePieceIndex ,puzzleSideSize );                           
    }
    return hn;
}
int calculateFnUsingManhattanDistance(state_holder& state , state_holder& goalState ,smallInt stateGn,int puzzleSideSize){
    return stateGn + calculateHnUsingManhattanDistance(state , goalState , puzzleSideSize);
}
int calculateLinear(state_holder state, int puzzleSideSize)
{
    auto conflicts = 0;

    {
        bool in_col[puzzleSideSize * puzzleSideSize] = { }; // set to true if this value is in the correct column
        bool in_row[puzzleSideSize * puzzleSideSize] = { }; // set to true if this value is in the correct row

        for (auto y = 0; y != puzzleSideSize; ++y)
        {
            for (auto x = 0; x != puzzleSideSize; ++x)
            {
                auto i = y * puzzleSideSize + x;

                auto bx = state[i] % puzzleSideSize;
                auto by = state[i] / puzzleSideSize;

                in_col[i] = (bx == x);
                in_row[i] = (by == y);
            }
        }

        for (auto y = 0; y != puzzleSideSize; ++y)
        {
            for (auto x = 0; x != puzzleSideSize; ++x)
            {
                auto i = y * puzzleSideSize + x;

                if (state[i] == 0) // ignore the empty space
                    continue;

                if (in_col[i])
                {
                    for (auto r = y; r != puzzleSideSize; ++r) // check down for conflicts
                    {
                        auto j = r * puzzleSideSize + x;

                        if (state[j] == 0) // ignore the empty space
                            continue;

                        if (in_col[j] && state[j] < state[i])
                            ++conflicts;
                    }
                }

                if (in_row[i])
                {
                    for (auto c = x; c != puzzleSideSize; ++c) // check right for conflicts
                    {
                        auto j = y * puzzleSideSize + c;

                        if (state[j] == 0) // ignore the empty space
                            continue;

                        if (in_row[j] && state[j] < state[i])
                            ++conflicts;
                    }
                }
            }
        }
    }
    // std::cout << "conflicts: " <<  conflicts << std::endl;
    return 2 * conflicts;
}
int calculateFnUsingManhattanDistanceAndLC(state_holder& state , state_holder& goalState ,smallInt stateGn,int puzzleSideSize){
    return stateGn + calculateHnUsingManhattanDistance(state , goalState , puzzleSideSize)+calculateLinear(state , puzzleSideSize);
}