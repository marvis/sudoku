all:
	g++ -o sudoku sudoku.cpp
	./sudoku < input
