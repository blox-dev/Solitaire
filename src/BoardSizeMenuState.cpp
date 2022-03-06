#include "BoardSizeMenuState.h"
#include "Board.h"
#include "GameEngine.h"

#include <SDL/SDL.h>
#include "BoardShapeMenuState.h"

BoardSizeMenuState BoardSizeMenuState::mBoardSizeMenuState;

void BoardSizeMenuState::Init()
{
	infoText.loadFromRenderedText("Choose board size: ");

	mContinueButton.init(
		"Continue",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mBackButton.init(
		"Back",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mValueSlider.init(MIN_BOARD_SIZE, MAX_BOARD_SIZE);

	gBoard.setBoardSize(4);

	InitPos();
}

void BoardSizeMenuState::InitPos()
{
	mContinueButton.setPos(
		gScreenWidth * 0.8,
		gScreenHeight * 0.85,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);

	mValueSlider.setPos(5 + infoText.tWidth, 5, gScreenWidth - 10 - infoText.tWidth, infoText.tHeight);

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

void BoardSizeMenuState::Cleanup()
{
	infoText.free();
}

void BoardSizeMenuState::Pause()
{
}

void BoardSizeMenuState::Resume()
{
	InitPos();
}

void BoardSizeMenuState::HandleEvents(GameEngine* game)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
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
}



void BoardSizeMenuState::Update(GameEngine* game)
{
	mValueSlider.update();

	gBoard.setBoardSize(mValueSlider.getValue());

	if (mContinueButton.clicked())
	{
		gBoard.setBoardSize(mValueSlider.getValue());
		gBoard.reset();

		game->PushState(BoardShapeMenuState::Instance());
	}
	if (mBackButton.clicked())
	{
		game->PopState();
	}
}

void BoardSizeMenuState::Draw(GameEngine* game)
{
	infoText.render(5, 5, 1.0f);

	mValueSlider.render();

	gBoard.draw(BoardMode::EMPTY);

	mContinueButton.render();
	mBackButton.render();
}
