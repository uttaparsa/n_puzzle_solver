PLUS     := g++
FLAGS    := -O3
all: search

search: obj/main.o obj/astar_manhattandistance10-stable.o obj/dfs3-stable.o obj/VectorState.o obj/BitsetHash.o obj/bds2-stable.o obj/idastar3-stable.o obj/ManhattanDistance.o obj/bfs3.o
	$(PLUS) $(FLAGS) -o $@ $^

obj/main.o: src/main.cpp
	$(PLUS) $(FLAGS) -c src/main.cpp -o obj/main.o

obj/%.o: src/%.cpp
	$(PLUS) $(FLAGS) -c  $< -o $@

clean:
	rm obj/*
	



# g++ src/main.cpp src/astar_manhattandistance10-stable.cpp src/dfs3-stable.cpp src/VectorState.cpp src/BitsetHash.cpp -o main
