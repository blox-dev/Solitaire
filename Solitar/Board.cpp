#include "Board.h"

Board::Board()
{
	mBoardSize = 0;
	for (int i = 0; i < MAX_BOARD_SIZE; ++i)
		for (int j = 0; j < MAX_BOARD_SIZE; ++j)
			mBoard[i][j] = 0;
}

Board::~Board()
{
}

void Board::drawBoard(int x0, int y0, int width, int height)
{
	//Render board
	SDL_Rect outlineRect = { x0, y0, width, height };
	SDL_SetRenderDrawColor(gRenderer, gBoardColor.r, gBoardColor.g, gBoardColor.b, gBoardColor.a);
	SDL_RenderFillRect(gRenderer, &outlineRect);

	//Draw black outline
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &outlineRect);

	//Draw blue horizontal line
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

	int cellWidth = width / mBoardSize;
	int cellHeight = height / mBoardSize;

	for (int i = x0; i < x0 + width; i += cellWidth)
		for (int j = y0; j < y0 + height; j += cellHeight)
		{
			SDL_RenderDrawLine(gRenderer, i, j, i, j + width);
			SDL_RenderDrawLine(gRenderer, i, j, i + height, j);
		}
}

bool Board::isValid()
{
	bool oneEmpty = false, oneFilled = false;

	for (int i = 0; i < mBoardSize; ++i)
		for (int j = 0; j < mBoardSize; ++j)
		{
			if (mBoard[i][j] == 0)
				oneEmpty = true;
			if (mBoard[i][j] == 1)
				oneFilled = true;

			if (oneEmpty && oneFilled)
				return true;
		}

	return false;
}

bool Board::hasPossibleMovesLeft()
{
	return true;
}

void Board::fill()
{
	for (int i = 0; i < mBoardSize; ++i)
		for (int j = 0; j < mBoardSize; ++j)
			if(mBoard[i][j] == 0)
				mBoard[i][j] = 1;
}

void Board::empty()
{
	for (int i = 0; i < mBoardSize; ++i)
		for (int j = 0; j < mBoardSize; ++j)
			if (mBoard[i][j] == 1)
				mBoard[i][j] = 0;
}