#include "PlayGameState.h"

#include "GameEngine.h"
#include "Board.h"

#include <SDL/SDL.h>
#include "MenuState.h"

PlayGameState PlayGameState::mPlayGameState;

void PlayGameState::Init()
{
	printf("BoardShapeMenuState Init\n");

	mPlayer1Text.loadFromRenderedText("It's player 1's turn");
	mPlayer2Text.loadFromRenderedText("It's player 2's turn");
	mComputerText.loadFromRenderedText("It's the computer's turn");

	if(gBoard.isComputerPlaying())
		mScoreText.loadFromRenderedText("You 0 : 0 Comp");
	else mScoreText.loadFromRenderedText("P1 0 : 0 P2");

	mPlayer1WinText.loadFromRenderedText("Player 1 wins!");
	mPlayer2WinText.loadFromRenderedText("Player 2 wins!");
	mComputerWinText.loadFromRenderedText("The computer wins!");
	mDrawText.loadFromRenderedText("It's a draw!");

	mBackButton.init(
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked,
		"Main menu"
	);

	mBackButton.setPos(
		gScreenWidth * 0.75,
		gScreenHeight * 0,
		gScreenWidth * 0.2,
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

	// free text
	mPlayer1Text.free();
	mPlayer2Text.free();
	mComputerText.free();

	mDrawText.free();
	mPlayer1WinText.free();
	mPlayer2WinText.free();
	mComputerWinText.free();

	mScoreText.free();
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

	gBoard.update(BoardMode::PLAY);

	//if scores need to be updated
	if (gBoard.updateScores()) {
		int score1 = gBoard.getPlayer1Score();
		int score2 = gBoard.getPlayer2Score();
		if (gBoard.isComputerPlaying())
			mScoreText.loadFromRenderedText("You " + std::to_string(score1) + " : " + std::to_string(score2) + " Comp", SDL_Color{ 255,255,255,255 });
		else mScoreText.loadFromRenderedText("P1 " + std::to_string(score1) + " : " + std::to_string(score2) + " P2", SDL_Color{ 255,255,255,255 });
		gBoard.setUpdateScores(false);

		//SDL_RenderPresent(gRenderer);
	}

	if (mBackButton.clicked())
	{
		game->ChangeState(MenuState::Instance());
	}

	
}

void PlayGameState::Update(GameEngine* game)
{
}

void PlayGameState::Draw(GameEngine* game)
{
	int h = game->getScreenHeight();
	int w = game->getScreenWidth();

	SDL_Renderer* r = gRenderer;

	if (!gBoard.hasNoMovesLeft())
	{
		//game is not over
		if (gBoard.getCurrentPlayer() == 0)
			mPlayer1Text.render(5, 5, 1.0f);

		if (gBoard.getCurrentPlayer() == 1)
		{
			if (gBoard.isComputerPlaying())
			{
				mComputerText.render(5, 5, 1.0f);
			}
			else {
				mPlayer2Text.render(5, 5, 1.0f);
			}
		}
	}
	else {
		//game is over
		//TODO: Only do this once
		mBackButton.setActive(true);

		int score1 = gBoard.getPlayer1Score();
		int score2 = gBoard.getPlayer2Score();

		if (score1 > score2)
		{
			mPlayer1WinText.render(5, 5, 1.0f);
		}
		else if (score2 > score1)
		{
			if (gBoard.isComputerPlaying())
			{
				mComputerWinText.render(5, 5, 1.0f);
			}
			else {
				mPlayer2WinText.render(5, 5, 1.0f);
			}
		}
		else {
			mDrawText.render(5, 5, 1.0f);
		}
	}

	//score text
	mScoreText.render(gScreenWidth * 0.5, 5, 1.0f);

	gBoard.draw(BoardMode::PLAY);

	mBackButton.render();
}
