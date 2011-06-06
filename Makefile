all:
	g++ -g -o sudoku sudoku.cpp
	./sudoku < input
