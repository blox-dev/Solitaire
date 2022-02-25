#include "PlayGameState.h"

#include "GameEngine.h"
#include "Board.h"

#include <SDL/SDL.h>
#include "MenuState.h"

PlayGameState PlayGameState::mPlayGameState;

void PlayGameState::Init()
{
	printf("BoardShapeMenuState Init\n");

	//buttons
	mButton.loadFromFile("textures/Button.png");
	mButtonHighlighted.loadFromFile("textures/ButtonHighlighted.png");
	mButtonPressed.loadFromFile("textures/ButtonPressed.png");

	SDL_Color color{ 255,255,255,255 };

	mPlayer1Text.loadFromRenderedText("It's player 1's turn", color);
	mPlayer2Text.loadFromRenderedText("It's player 2's turn", color);
	mComputerText.loadFromRenderedText("It's the computer's turn", color);

	mPlayer1WinText.loadFromRenderedText("Player 1 wins!", color);
	mPlayer1WinText.loadFromRenderedText("Player 2 wins!", color);
	mComputerWinText.loadFromRenderedText("The computer wins!", color);

	mBackButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Main menu");

	mBackButton.setPos(
		gScreenWidth * 0.8,
		gScreenHeight * 0,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);

	mBackButton.setActive(false);

	gBoard.setPosition(
		gScreenWidth * 0.1,
		gScreenHeight * 0.1,
		gScreenWidth * 0.8,
		gScreenHeight * 0.8
	);
}

void PlayGameState::Cleanup()
{
	printf("BoardShapeMenuState Cleanup\n");

	mButton.free();
	mButtonPressed.free();
	mButtonHighlighted.free();
	mButtonBlocked.free();


	// free text
	mPlayer1Text.free();
	mPlayer2Text.free();
	mComputerText.free();
}

void PlayGameState::Pause()
{
}

void PlayGameState::Resume()
{
}

void PlayGameState::HandleEvents(GameEngine* game)
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

	if (mBackButton.clicked())
	{
		game->ChangeState(MenuState::Instance());
	}

	if (gBoard.hasNoMovesLeft())
	{
		//TODO: only do this once
		mBackButton.setActive(true);
	}

	gBoard.update(BoardMode::PLAY);
}

void PlayGameState::Update(GameEngine* game)
{
}

void PlayGameState::Draw(GameEngine* game)
{
	int h = game->getScreenHeight();
	int w = game->getScreenWidth();

	SDL_Renderer* r = gRenderer;

	if(gBoard.getCurrentPlayer() == 0)
		mPlayer1Text.render(
			5,
			5,
			gScreenWidth * 0.4,
			gScreenHeight * 0.05
		);

	if(gBoard.getCurrentPlayer() == 1)
		mPlayer2Text.render(
			5,
			5,
			gScreenWidth * 0.4,
			gScreenHeight * 0.05
		);

	if(false)
		mComputerText.render(
			5,
			5,
			gScreenWidth * 0.4,
			gScreenHeight * 0.05
		);

	int x0 = w * 0.1;
	int y0 = h * 0.1;

	gBoard.draw(BoardMode::PLAY);

	mBackButton.render();
}
