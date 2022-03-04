#include "BoardPiecesMenuState.h"

#include "GameEngine.h"
#include "Board.h"

#include <SDL/SDL.h>
#include "PlayGameState.h"

BoardPiecesMenuState BoardPiecesMenuState::mBoardPiecesMenuState;

void BoardPiecesMenuState::Init()
{
	printf("BoardShapeMenuState Init\n");

	infoText.loadFromRenderedText("Place pieces on the board (at least one, at least one empty tile)");

	mBackButton.init(
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked,
		"Back");

	mEmptyButton.init(
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked,
		"Empty"
	);

	mFillButton.init(
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked,
		"Fill"
	);

	mContinueButton.init(
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked,
		"Start game");
	
	//-------------

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

	mContinueButton.setBlocked(true);

	gBoard.setPosition(
		gScreenWidth * 0.1,
		gScreenHeight * 0.1,
		gScreenWidth * 0.75,
		gScreenHeight * 0.75
	);
}

void BoardPiecesMenuState::Cleanup()
{
	printf("BoardShapeMenuState Cleanup\n");

	infoText.free();
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

void BoardPiecesMenuState::Update(GameEngine* game)
{
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
