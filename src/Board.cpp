#include "Board.h"
#include <cmath>

Board::Board()
{
	mCurrentPlayer = 0;
	mBoardSize = 0;
	for (int i = 0; i < MAX_BOARD_SIZE; ++i)
		for (int j = 0; j < MAX_BOARD_SIZE; ++j)
			mBoard[i][j] = 0;
}

Board::~Board()
{
}

void Board::draw(BoardMode mode)
{
	//Render board
	SDL_Rect outlineRect = { x, y, bWidth, bHeight };
	SDL_SetRenderDrawColor(gRenderer, gBoardColor.r, gBoardColor.g, gBoardColor.b, gBoardColor.a);
	SDL_RenderFillRect(gRenderer, &outlineRect);

	//Draw white outline
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &outlineRect);

	//Set to white
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

	int cellWidth = bWidth / mBoardSize;
	int cellHeight = bHeight / mBoardSize;

	int i, j, row, col;

	//draw horizontal lines
	for (i = y; i < y + bHeight; i += cellHeight)
		SDL_RenderDrawLine(gRenderer, x, i, x + bWidth, i);

	//draw vertical lines
	for (i = x; i < x + bWidth; i += cellWidth)
		SDL_RenderDrawLine(gRenderer, i, y, i, y + bHeight);

	//state specific drawing
	switch (mode) {
	case BoardMode::EMPTY:
		//nothing more
		break;
	case BoardMode::REMOVE:
		//switch to red for missing tiles
		SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);

		for (row = 0; row < mBoardSize; ++row)
			for (col = 0; col < mBoardSize; ++col)
				if (mBoard[row][col] == 2) {

					i = x + col * cellWidth;
					j = y + row * cellHeight;

					// draw red cross
					SDL_RenderDrawLine(gRenderer, i + cellWidth / 5, j + cellHeight / 5, i + cellWidth * 0.8, j + cellHeight * 0.8);
					SDL_RenderDrawLine(gRenderer, i + cellWidth * 0.8, j + cellHeight / 5, i + cellWidth / 5, j + cellHeight * 0.8);
				}

		drawPieces();

		//reset drawing color 
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

		break;

	case BoardMode::PLACE:

		for (row = 0; row < mBoardSize; ++row)
			for (col = 0; col < mBoardSize; ++col)
			{
				if (mBoard[row][col] == 2) {

					i = x + col * cellWidth;
					j = y + row * cellHeight;

					//switch to red for missing tiles
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);

					// draw red cross
					SDL_RenderDrawLine(gRenderer, i + cellWidth / 5, j + cellHeight / 5, i + cellWidth * 0.8, j + cellHeight * 0.8);
					SDL_RenderDrawLine(gRenderer, i + cellWidth * 0.8, j + cellHeight / 5, i + cellWidth / 5, j + cellHeight * 0.8);
				}
			}

		drawPieces();

		//reset drawing color 
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

		break;

	case BoardMode::PLAY:
		for (row = 0; row < mBoardSize; ++row)
			for (col = 0; col < mBoardSize; ++col)
			{
				if (mBoard[row][col] == 2) {

					i = x + col * cellWidth;
					j = y + row * cellHeight;

					//switch to red for missing tiles
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);

					// draw red cross
					SDL_RenderDrawLine(gRenderer, i + cellWidth / 5, j + cellHeight / 5, i + cellWidth * 0.8, j + cellHeight * 0.8);
					SDL_RenderDrawLine(gRenderer, i + cellWidth * 0.8, j + cellHeight / 5, i + cellWidth / 5, j + cellHeight * 0.8);
				}
			}

		drawPieces();

		if (isHighlight) {
			drawHighlightedPieces();
		}

		//reset drawing color 
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

		break;
	}

	//background color
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
}

void Board::drawPieces()
{
	// TODO: if all the vertices are in one array, redraw isn't always necessary (and it's costly)
	if (renderPieces) {
		//renderPieces = false;

		int cellWidth = bWidth / mBoardSize;
		int cellHeight = bHeight / mBoardSize;

		double radiusx = cellWidth / 3.0;
		double radiusy = cellHeight / 3.0;

		for (int row = 0; row < mBoardSize; ++row)
			for (int col = 0; col < mBoardSize; ++col)
				if (mBoard[row][col] == 1) {

					// add circle
					float cx = x + col * cellWidth + cellWidth / 2.0;
					float cy = y + row * cellHeight + cellHeight / 2.0;

					//first vertex is the center of the circle
					mPieceVertices[0].position = { cx,cy };
					mPieceVertices[0].color = gPieceColor;

					int pieceVertexId = 1;

					// create TRIANGLES_PER_CIRCLE vertices and store them
					while (pieceVertexId < TRIANGLES_PER_CIRCLE + 1)
					{
						float theta = double_pi * (pieceVertexId - 1) / TRIANGLES_PER_CIRCLE;

						mPieceVertices[pieceVertexId].position = { static_cast<float>(radiusx * cos(theta)) + cx, static_cast<float>(radiusy * sin(theta)) + cy };
						mPieceVertices[pieceVertexId].color = gPieceColor;

						pieceVertexId++;
					}

					// copy the vertices into big array(representing one piece) with triangles
					// triangle 1: center, p1, p2
					// triangle 2: center, p2, p3
					// ...
					int i = 0, current = 1;
					while (i < 3 * TRIANGLES_PER_CIRCLE)
					{
						mVertices[i++] = mPieceVertices[0];
						mVertices[i++] = mPieceVertices[current];

						// TODO: make this better
						if (current + 1 <= TRIANGLES_PER_CIRCLE)
							mVertices[i++] = mPieceVertices[current + 1];
						else mVertices[i++] = mPieceVertices[1];

						current++;
					}

					//render one piece
					SDL_RenderGeometry(gRenderer, nullptr, &mVertices[0], 3 * TRIANGLES_PER_CIRCLE, nullptr, 0);
				}
	}
}

void Board::setPosition(int x0, int y0, int width, int height)
{
	x = x0;
	y = y0;

	//dirty fix
	if (mBoardSize != 0)
		bWidth = (width / mBoardSize) * mBoardSize;
	else bWidth = width;

	if (mBoardSize != 0)
		bHeight = (height / mBoardSize) * mBoardSize;
	else bHeight = height;
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
				return !hasNoMovesLeft();
		}

	return false;
}

void Board::update(BoardMode mode)
{
	if (mode == BoardMode::PLAY)
		if (mCurrentPlayer == 1 && mIsComputerPlaying)
		{
			//it's the computer's turn
			makeComputerMove();
			mCurrentPlayer = 0;
		}
	if (gInputManager.isMouseInBox(x, y, bWidth, bHeight) && gInputManager.wasKeyReleased(SDL_BUTTON_LEFT)) {

		SDL_Point mouseCoords = gInputManager.getMouseCoords();
		int row = (mouseCoords.y - y) / (bHeight / mBoardSize);
		int col = (mouseCoords.x - x) / (bWidth / mBoardSize);

		if (0 <= row && row < MAX_BOARD_SIZE && 0 <= col && col < MAX_BOARD_SIZE)
		{
			switch (mode) {
			case BoardMode::EMPTY:
				//do nothing
				break;
			case BoardMode::REMOVE:
				// setting unreachable tiles
				// 0 to 2 and 2 to 0
				mBoard[row][col] = 2 - mBoard[row][col];
				break;
			case BoardMode::PLACE:
				// setting initial board by placing or removing pieces
				// 0 to 1 and 1 to 0
				if (mBoard[row][col] != 2)
					mBoard[row][col] = 1 - mBoard[row][col];
				break;
			case BoardMode::PLAY:
				//game logic
				// if user just clicks around
				// and selects a valid piece
				if (!moveStarted && mBoard[row][col] == 1)
				{
					clearHighLight();
					if (highlight(row, col))
					{
						isHighlight = true;
						mSelectedPiece = { row,col };
					}
				}
				// if the player presses a highlighted piece
				if (isHighlight && mBoard[row][col] == 3)
				{
					isHighlight = false;
					mScores[mCurrentPlayer] ++;

					mUpdateScores = true;

					clearHighLight();

					makeMove(mSelectedPiece.x, mSelectedPiece.y, row, col);

					// switch players
					mCurrentPlayer = 1 - mCurrentPlayer;

					moveStarted = false;
					// if there are however more moves to be made
					if (highlight(row, col))
					{
						moveStarted = true;
						isHighlight = true;
						mSelectedPiece = { row,col };

						//switch the players again so that the current player can continue his play
						mCurrentPlayer = 1 - mCurrentPlayer;
					}
				}
				break;
			}
		}
	}
}

bool Board::hasNoMovesLeft()
{
	// returns true if there is at least one move left
	for (int row = 0; row < mBoardSize; ++row)
		for (int col = 0; col < mBoardSize; ++col)
			if (mBoard[row][col] == 1)
			{
				//TODO: code repeating, abstract this and highlight
				if (row + 2 < mBoardSize)
					if ((mBoard[row + 2][col] == 0 || mBoard[row + 2][col] == 3) && mBoard[row + 1][col] == 1) {
						return false;
					}

				//up
				if (row - 2 >= 0)
					if ((mBoard[row - 2][col] == 0 || mBoard[row - 2][col] == 3) && mBoard[row - 1][col] == 1) {
						return false;
					}

				//right
				if (col + 2 < mBoardSize)
					if ((mBoard[row][col + 2] == 0 || mBoard[row][col + 2] == 3) && mBoard[row][col + 1] == 1) {
						return false;
					}

				//left
				if (col - 2 >= 0)
					if ((mBoard[row][col - 2] == 0 || mBoard[row][col - 2] == 3) && mBoard[row][col - 1] == 1) {
						return false;
					}
			}

	return true;
}

void Board::fill()
{
	for (int i = 0; i < mBoardSize; ++i)
		for (int j = 0; j < mBoardSize; ++j)
			if (mBoard[i][j] == 0)
				mBoard[i][j] = 1;
}

void Board::empty()
{
	for (int i = 0; i < mBoardSize; ++i)
		for (int j = 0; j < mBoardSize; ++j)
			if (mBoard[i][j] == 1)
				mBoard[i][j] = 0;
}

void Board::reset()
{
	mCurrentPlayer = 0;
	mScores[0] = 0;
	mScores[1] = 0;

	for (int i = 0; i < mBoardSize; ++i)
		for (int j = 0; j < mBoardSize; ++j)
			mBoard[i][j] = 0;
}

void Board::makeComputerMove()
{
	if (hasNoMovesLeft())
		return;

	int bestScore = 0;

	// encoded as such
	// 0 - move length
	// 1+ - direction in which to jump - 0 - N, 1 - E, 2 - S, 3 - W
	int currentMove[100]{ -1 };

	int bestMove[100]{ -1 };

	int bestX = 0, bestY = 0;

	bool bestChanged = false;

	//compute the best move
	for (int i = 0; i < mBoardSize; ++i)
		for (int j = 0; j < mBoardSize; ++j)
			if (mBoard[i][j] == 1)
			{
				findBestMoveFromPos(i, j, &currentMove[0], &bestMove[0], 1, bestChanged);
				if (bestChanged)
				{
					bestChanged = false;
					bestX = i;
					bestY = j;
				}
			}

	//do the move
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

	mUpdateScores = true;

	int currentX = bestX;
	int currentY = bestY;
	int nextX, nextY;
	for (int i = 1; i <= bestMove[0]; ++i)
	{
		nextX = currentX + di[bestMove[i]] * 2;
		nextY = currentY + dj[bestMove[i]] * 2;

		makeMove(currentX, currentY, nextX, nextY);
		mScores[1]++;

		SDL_Delay(750);
		draw(BoardMode::PLAY);
		SDL_RenderPresent(gRenderer);

		currentX = nextX;
		currentY = nextY;
	}
}
// initial step = 1
void Board::findBestMoveFromPos(int i, int j, int* currentMove, int* bestMove, int step, bool& bestChanged) {
	for (int k = 0; k < 4; ++k)
	{
		int nexti = i + 2 * di[k];
		int nextj = j + 2 * dj[k];

		//still inside the playing board
		if (nexti >= 0 && nexti < mBoardSize && nextj >= 0 && nextj < mBoardSize)
		{
			//can jump over there
			if (mBoard[i][j] == 1 &&
				mBoard[i + (nexti - i) / 2][j + (nextj - j) / 2] == 1 &&
				(mBoard[nexti][nextj] == 0 || mBoard[nexti][nextj] == 3))
			{
				currentMove[step] = k;
				makeMove(i, j, nexti, nextj);
				if (step > bestMove[0])
				{
					bestChanged = true;
					//found new best
					bestMove[0] = step;

					for (int i = 1; i <= step; ++i)
						bestMove[i] = currentMove[i];
				}
				findBestMoveFromPos(nexti, nextj, currentMove, bestMove, step + 1, bestChanged);
				unmakeMove(i, j, nexti, nextj);
				currentMove[step] = 0;
			}
		}
	}
}

void Board::clearHighLight()
{
	for (int i = 0; i < mBoardSize; ++i)
		for (int j = 0; j < mBoardSize; ++j)
			if (mBoard[i][j] == 3)
				mBoard[i][j] = 0;
}

void Board::drawHighlightedPieces()
{
	int cellWidth = bWidth / mBoardSize;
	int cellHeight = bHeight / mBoardSize;

	// green
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);

	for (int row = 0; row < mBoardSize; ++row)
		for (int col = 0; col < mBoardSize; ++col)
			if (mBoard[row][col] == 3)
			{
				int i = x + col * cellWidth;
				int j = y + row * cellHeight;
				// draw green cross
				SDL_RenderDrawLine(gRenderer, i + cellWidth / 5, j + cellHeight / 5, i + cellWidth * 0.8, j + cellHeight * 0.8);
				SDL_RenderDrawLine(gRenderer, i + cellWidth * 0.8, j + cellHeight / 5, i + cellWidth / 5, j + cellHeight * 0.8);
			}

	//reset drawing color 
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
}

bool Board::highlight(int row, int col)
{
	bool possibleMove = false;

	//down
	if (row + 2 < mBoardSize)
		if (mBoard[row + 2][col] == 0 && mBoard[row + 1][col] == 1) {
			mBoard[row + 2][col] = 3;
			possibleMove = true;
		}

	//up
	if (row - 2 >= 0)
		if (mBoard[row - 2][col] == 0 && mBoard[row - 1][col] == 1) {
			mBoard[row - 2][col] = 3;
			possibleMove = true;
		}

	//right
	if (col + 2 < mBoardSize)
		if (mBoard[row][col + 2] == 0 && mBoard[row][col + 1] == 1) {
			mBoard[row][col + 2] = 3;
			possibleMove = true;
		}

	//left
	if (col - 2 >= 0)
		if (mBoard[row][col - 2] == 0 && mBoard[row][col - 1] == 1) {
			mBoard[row][col - 2] = 3;
			possibleMove = true;
		}

	// maybe mark somehow this is the selected piece
	//if (possibleMove)
	//	mBoard[row][col] = 123;

	return possibleMove;
}

void Board::makeMove(int startRow, int startCol, int destRow, int destCol)
{
	int difRow = destRow - startRow;
	int difCol = destCol - startCol;

	// shouldn't happen
	if (difRow && difCol) {
		printf("Both /= 0 Why\n");
		return;
	}
	// shouldn't happen
	if (!(difRow || difCol)) {
		printf("Both == 0 Why\n");
		return;
	}

	if (difCol == 0 && abs(difRow) == 2)
	{
		mBoard[startRow][startCol] = 0;
		mBoard[startRow + difRow / 2][startCol] = 0;
		mBoard[startRow + difRow][startCol] = 1;
	}
	else if (abs(difCol) == 2 && difRow == 0)
	{
		mBoard[startRow][startCol] = 0;
		mBoard[startRow][startCol + difCol / 2] = 0;
		mBoard[startRow][startCol + difCol] = 1;
	}
	else {
		// shouldn't happen
		printf("Not at all\n");
		printf("%d, %d\n", difRow, difCol);
	}
}

void Board::unmakeMove(int startRow, int startCol, int destRow, int destCol)
{
	int difRow = destRow - startRow;
	int difCol = destCol - startCol;

	// shouldn't happen
	if (difRow && difCol) {
		printf("Both /= 0 Why\n");
		return;
	}
	// shouldn't happen
	if (!(difRow || difCol)) {
		printf("Both == 0 Why\n");
		return;
	}

	if (difCol == 0 && abs(difRow) == 2)
	{
		mBoard[startRow][startCol] = 1;
		mBoard[startRow + difRow / 2][startCol] = 1;
		mBoard[startRow + difRow][startCol] = 0;
	}
	else if (abs(difCol) == 2 && difRow == 0)
	{
		mBoard[startRow][startCol] = 1;
		mBoard[startRow][startCol + difCol / 2] = 1;
		mBoard[startRow][startCol + difCol] = 0;
	}
	else {
		// shouldn't happen
		printf("Not at all\n");
		printf("%d, %d\n", difRow, difCol);
	}
}