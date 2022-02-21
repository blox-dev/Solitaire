#include "BoardPiecesMenuState.h"

#include "GameEngine.h"
#include "Board.h"

#include <SDL/SDL.h>

BoardPiecesMenuState BoardPiecesMenuState::mBoardPiecesMenuState;

void BoardPiecesMenuState::Init()
{
	printf("BoardShapeMenuState Init\n");

	//buttons
	mButton.loadFromFile("textures/Button.png");
	mButtonHighlighted.loadFromFile("textures/ButtonHighlighted.png");
	mButtonPressed.loadFromFile("textures/ButtonPressed.png");

	SDL_Color color{ 255,255,255,255 };

	infoText.loadFromRenderedText("Select initial state (1 <= Nr. of board pieces < " + std::to_string(gBoardSize*gBoardSize) + ")", color);

	mContinueButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Start game");
	mBackButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Back");

	mContinueButton.setPos(
		gScreenWidth * 0.8,
		gScreenHeight * 0.85,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);

	mBackButton.setPos(
		gScreenWidth * 0.05,
		gScreenHeight * 0.85,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);

	gBoard.setPosition(
		gScreenWidth * 0.1,
		gScreenHeight * 0.1,
		gScreenWidth * 0.8,
		gScreenHeight * 0.8
	);
}

void BoardPiecesMenuState::Cleanup()
{
	printf("BoardShapeMenuState Cleanup\n");

	mButton.free();
	mButtonPressed.free();
	mButtonHighlighted.free();
	mButtonBlocked.free();

	infoText.free();
	nText.free();
	inputTextTexture.free();
}

void BoardPiecesMenuState::Pause()
{
}

void BoardPiecesMenuState::Resume()
{
}

void BoardPiecesMenuState::HandleEvents(GameEngine* game)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			gInputManager.setMouseCoords(event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			gInputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			gInputManager.releaseKey(event.button.button);
			break;
		case SDL_QUIT:
			game->Quit();
			break;
		}
	}

	if (mContinueButton.clicked())
	{
		//game->PushState();
	}
	if (mBackButton.clicked())
	{
		game->PopState();
	}

	gBoard.update(BoardMode::PLACE);
}

void BoardPiecesMenuState::Update(GameEngine* game)
{
}

void BoardPiecesMenuState::Draw(GameEngine* game)
{
	int h = game->getScreenHeight();
	int w = game->getScreenWidth();

	SDL_Renderer* r = gRenderer;

	infoText.render(5, 5, w - 10, 25);

	int x0 = w * 0.1;
	int y0 = h * 0.1;

	gBoard.draw(BoardMode::PLACE);

	gBoard.drawPieces();


	mContinueButton.render();
	mBackButton.render();
}
