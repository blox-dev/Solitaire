#pragma once
#include "GameEngine.h";

// looks like a circle
constexpr int TRIANGLES_PER_CIRCLE = 20;

enum class BoardMode {
	REMOVE,
	PLACE,
	PLAY
};

// Board representation
// 0 - empty, 1 - filled, 2 - blocked
class Board {
public:
	Board();
	~Board();

	void setBoardSize(int boardSize) { mBoardSize = boardSize; };
	
	void draw(BoardMode mode);
	
	//use SDL_RenderGeometry to render the pieces (circles) with triangles
	//TODO: move this into Board.draw()
	void drawPieces();
	
	void setPosition(int x0, int y0, int width, int height);

	void setCell(int i, int j, int value) { mBoard[i][j] = value; };
	
	bool isValid();

	void update(BoardMode mode);

	bool hasPossibleMovesLeft();
	
	void fill();
	
	void empty();

private:
	double double_pi = 8.0 * atan(1);
	
	// bad name, represents one piece
	SDL_Vertex mVertices[3*TRIANGLES_PER_CIRCLE];

	// bad name, represents the individual points of one piece
	SDL_Vertex mPieceVertices[TRIANGLES_PER_CIRCLE + 1];

	bool renderPieces = true;

	int mBoardSize;
	int x = 0, y = 0, bWidth = 0, bHeight = 0;
	char mBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};