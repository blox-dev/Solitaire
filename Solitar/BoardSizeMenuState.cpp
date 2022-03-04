#include "BoardSizeMenuState.h"
#include "Board.h"
#include "GameEngine.h"

#include <SDL/SDL.h>
#include "BoardShapeMenuState.h"

BoardSizeMenuState BoardSizeMenuState::mBoardSizeMenuState;

void BoardSizeMenuState::Init()
{
	printf("BoardSizeMenuState Init\n");


	infoText.loadFromRenderedText("Choose board size: ");

	mContinueButton.init(
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked,
		"Continue"
	);

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

	mValueSlider.init(MIN_BOARD_SIZE, MAX_BOARD_SIZE);

	mValueSlider.setPos(5 + infoText.tWidth, 5, gScreenWidth - 10 - infoText.tWidth, infoText.tHeight);

	mBackButton.setPos(
		gScreenWidth * 0.05,
		gScreenHeight * 0.85,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);

	gBoard.setBoardSize(4);

	gBoard.setPosition(
		gScreenWidth * 0.1,
		gScreenHeight * 0.1,
		gScreenWidth * 0.75,
		gScreenHeight * 0.75
	);

}

void BoardSizeMenuState::Cleanup()
{
	printf("BoardSizeMenuState Cleanup\n");

	infoText.free();
}

void BoardSizeMenuState::Pause()
{
}

void BoardSizeMenuState::Resume()
{
}

void BoardSizeMenuState::HandleEvents(GameEngine* game)
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

	mValueSlider.update();

	gBoard.setBoardSize(mValueSlider.getValue());

	if (mContinueButton.clicked())
	{
		gBoardSize = mValueSlider.getValue();
		gBoard.setBoardSize(mValueSlider.getValue());
		gBoard.reset();

		game->PushState(BoardShapeMenuState::Instance());
	}
	if (mBackButton.clicked())
	{
		game->PopState();
	}
}



void BoardSizeMenuState::Update(GameEngine* game)
{
}

void BoardSizeMenuState::Draw(GameEngine* game)
{
	infoText.render(5, 5, 1.0f);

	mValueSlider.render();

	gBoard.draw(BoardMode::EMPTY);

	mContinueButton.render();
	mBackButton.render();
}
