#pragma once
#include "GameEngine.h";


// Board representation
// 0 - empty, 1 - filled, 2 - blocked
class Board {
public:
	Board();
	~Board();

	void setBoardSize(int boardSize) { mBoardSize = boardSize; };
	
	void drawBoard(int x0, int y0, int width, int height);
	
	void setCell(int i, int j, int value) { mBoard[i][j] = value; };
	
	bool isValid();

	bool hasPossibleMovesLeft();
	
	void fill();
	
	void empty();

private:
	int mBoardSize;
	char mBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};