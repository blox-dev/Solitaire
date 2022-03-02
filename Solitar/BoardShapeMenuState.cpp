#include "BoardShapeMenuState.h"

#include "GameEngine.h"
#include "Board.h"

#include <SDL/SDL.h>
#include "BoardPiecesMenuState.h"

BoardShapeMenuState BoardShapeMenuState::mBoardShapeMenuState;

void BoardShapeMenuState::Init()
{
	printf("BoardShapeMenuState Init\n");

	SDL_Color color{ 255,255,255,255 };

	infoText.loadFromRenderedText("Select unplayable tiles:", color);
	
	mContinueButton.init(
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked,
		"Continue");

	mBackButton.init(
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked,
		"Back"
	);

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
		gScreenWidth * 0.75,
		gScreenHeight * 0.75
	);
}

void BoardShapeMenuState::Cleanup()
{
	printf("BoardShapeMenuState Cleanup\n");

	infoText.free();
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
		game->PushState(BoardPiecesMenuState::Instance());
	}
	if (mBackButton.clicked())
	{
		game->PopState();
	}

	gBoard.update(BoardMode::REMOVE);
}

void BoardShapeMenuState::Update(GameEngine* game)
{
}

void BoardShapeMenuState::Draw(GameEngine* game)
{
	int h = game->getScreenHeight();
	int w = game->getScreenWidth();

	SDL_Renderer* r = gRenderer;

	infoText.render(5, 5, 1.0f);

	gBoard.draw(BoardMode::REMOVE);

	mContinueButton.render();
	mBackButton.render();
}