# N_PUZZLE SOLVER

This project is n puzzle solver with blind and heuritic searchy algorithms written in c++

## Getting Started
After installing you can run project with this command in terminal:
```
./search <inputfile> -m <search-method>
```  
Available search methods:
bfs
dfs
bds
idastar
astar
Input file format first line contains puzzle side size for exammple 3 for 8-puzzle and 4 for 15-puzzle and currently not supporting puzzles
bigger than 15-puzzle.
Two following lines contain puzzle starting state numbers and goal state numbers respectively.
Input file examples are available in project root.
### Prerequisites

Any c plus plus compiler but g++ is preferred.

### Installing


```
git clone https://github.com/uttaparsa/n_puzzle_solver.git
```
```
cd n_puzzle_solver
```
```
make
```


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

