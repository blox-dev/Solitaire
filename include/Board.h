#pragma once

#include "GameEngine.h"
#include <SDL/SDL.h>
#include <cmath>

// looks like a circle
constexpr int TRIANGLES_PER_CIRCLE = 20;

enum class BoardMode {
	EMPTY,
	REMOVE,
	PLACE,
	PLAY,
};

// Board representation
// 0 - empty, 1 - filled, 2 - blocked, 3 - highlighted
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

	bool hasNoMovesLeft();

	// fill all available spaces with pieces
	void fill();

	// remove all pieces
	void empty();

	// reset everything to 0
	void reset();

	int getCurrentPlayer() { return mCurrentPlayer; }
	int getPlayer1Score() { return mScores[0]; }
	int getPlayer2Score() { return mScores[1]; }

	//returns true if the scores need to be updated
	bool updateScores() { return mUpdateScores; }

	void setUpdateScores(bool updateScores) { mUpdateScores = updateScores; }

	bool isComputerPlaying() { return mIsComputerPlaying; }
	void setComputerPlaying(bool isComputerPlaying) { mIsComputerPlaying = isComputerPlaying; }

	void makeComputerMove();
private:
	bool moveStarted = false;
	bool mIsComputerPlaying = false;

	int mScores[2] = { 0,0 };
	bool mUpdateScores = false;
	int mCurrentPlayer = 0;
	SDL_Point mSelectedPiece;

	void clearHighLight();

	void drawHighlightedPieces();

	// returns true if there are places where the piece can move
	bool highlight(int row, int col);

	void makeMove(int startRow, int startCol, int destRow, int destCol);
	void unmakeMove(int startRow, int startCol, int destRow, int destCol);

	void findBestMoveFromPos(int i, int j, int* currentMove, int* bestMove, int step, bool& bestChanged);

	const double double_pi = 8.0 * atan(1);
	const int di[4] = { -1,0,1,0 };
	const int dj[4] = { 0,1,0,-1 };

	// bad name, represents one piece
	SDL_Vertex mVertices[3 * TRIANGLES_PER_CIRCLE];


	// bad name, represents the individual points of one piece
	SDL_Vertex mPieceVertices[TRIANGLES_PER_CIRCLE + 1];

	bool renderPieces = true;
	bool isHighlight = false;

	int mBoardSize;
	int x = 0, y = 0, bWidth = 0, bHeight = 0;
	char mBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};