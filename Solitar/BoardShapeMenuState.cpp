#include "BoardShapeMenuState.h"

#include "GameEngine.h"
#include "Board.h"

#include <SDL/SDL.h>

BoardShapeMenuState BoardShapeMenuState::mBoardShapeMenuState;

void BoardShapeMenuState::Init()
{
	printf("BoardSizeMenuState Init\n");

	//buttons
	mButton.loadFromFile("textures/Button.png");
	mButtonHighlighted.loadFromFile("textures/ButtonHighlighted.png");
	mButtonPressed.loadFromFile("textures/ButtonPressed.png");

	SDL_Color color{ 255,255,255,255 };

	infoText.loadFromRenderedText("Choose board shape:", color);
	
	mContinueButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Continue");
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
}

void BoardShapeMenuState::Cleanup()
{
	printf("BoardSizeMenuState Cleanup\n");

	mButton.free();
	mButtonPressed.free();
	mButtonHighlighted.free();
	mButtonBlocked.free();

	infoText.free();
	nText.free();
	inputTextTexture.free();
}

void BoardShapeMenuState::Pause()
{
}

void BoardShapeMenuState::Resume()
{
}

void BoardShapeMenuState::HandleEvents(GameEngine* game)
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
}

void BoardShapeMenuState::Update(GameEngine* game)
{
}

void BoardShapeMenuState::Draw(GameEngine* game)
{
	int h = game->getScreenHeight();
	int w = game->getScreenWidth();

	SDL_Renderer* r = gRenderer;

	infoText.render(5, 5, w - 10, 25);
	
	int x0 = w * 0.1;
	int y0 = h * 0.1;

	gBoard.drawBoard(x0, y0, w * 0.8, h * 0.8);

	mContinueButton.render();
	mBackButton.render();
}