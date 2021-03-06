#include "BoardPiecesMenuState.h"

#include "GameEngine.h"
#include "Board.h"

#include <SDL/SDL.h>
#include "PlayGameState.h"

BoardPiecesMenuState BoardPiecesMenuState::mBoardPiecesMenuState;

void BoardPiecesMenuState::Init()
{
	infoText.loadFromRenderedText("Place pieces on the board (at least one, at least one empty tile)");

	mBackButton.init(
		"Back",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mEmptyButton.init(
		"Empty",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mFillButton.init(
		"Fill",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mContinueButton.init(
		"Start game",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mContinueButton.setBlocked(true);

	InitPos();
}

void BoardPiecesMenuState::InitPos()
{
	mBackButton.setPos(
		gScreenWidth * 0.05,
		gScreenHeight * 0.85,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);

	mEmptyButton.setPos(
		gScreenWidth * 0.30,
		gScreenHeight * 0.85,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);

	mFillButton.setPos(
		gScreenWidth * 0.55,
		gScreenHeight * 0.85,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);

	mContinueButton.setPos(
		gScreenWidth * 0.8,
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

void BoardPiecesMenuState::Cleanup()
{
	infoText.free();
}

void BoardPiecesMenuState::Pause()
{
}

void BoardPiecesMenuState::Resume()
{
	InitPos();
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
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//Get new dimensions and repaint on window size change
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				gScreenWidth = event.window.data1;
				gScreenHeight = event.window.data2;
				InitPos();
				SDL_RenderPresent(gRenderer);
				break;

				//Repaint on exposure
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_RenderPresent(gRenderer);
				break;
			}
			break;
		case SDL_QUIT:
			game->Quit();
			break;
		}
	}
}

void BoardPiecesMenuState::Update(GameEngine* game)
{
	if (gBoard.isValid())
	{
		mContinueButton.setBlocked(false);
	}
	else mContinueButton.setBlocked(true);

	if (mEmptyButton.clicked())
	{
		gBoard.empty();
	}

	if (mFillButton.clicked())
	{
		gBoard.fill();
	}

	if (mContinueButton.clicked())
	{
		game->PushState(PlayGameState::Instance());
	}
	if (mBackButton.clicked())
	{
		gBoard.empty();
		game->PopState();
	}

	gBoard.update(BoardMode::PLACE);
}

void BoardPiecesMenuState::Draw(GameEngine* game)
{
	infoText.render(5, 5, 1.0f);

	gBoard.draw(BoardMode::PLACE);

	mBackButton.render();

	mEmptyButton.render();

	mFillButton.render();

	mContinueButton.render();
}
