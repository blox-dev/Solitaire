#include "MenuState.h"

#include "GameEngine.h"
#include "BoardSizeMenuState.h"
#include "InstructionsState.h"
#include "SettingsState.h"
#include "Board.h"

#include <SDL/SDL.h>

MenuState MenuState::mMenuState;

void MenuState::Init()
{
	//title
	mTitleImage.loadFromFile("textures/Title.png");

	//buttons
	mOnePlayerButton.init(
		"1 Player",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mTwoPlayerButton.init(
		"2 Players",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mInstructionsButton.init(
		"Instructions",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mSettingsButton.init(
		"Settings",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	mExitButton.init(
		"Quit game",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	InitPos();
}

void MenuState::InitPos()
{
	int divHeight = gScreenHeight / 13;
	int divWidth = gScreenWidth / 6;

	mOnePlayerButton.setPos(
		gScreenWidth / 2 - divWidth,
		3 * divHeight,
		divWidth * 2,
		divHeight
	);

	mTwoPlayerButton.setPos(
		gScreenWidth / 2 - divWidth,
		5 * divHeight,
		divWidth * 2,
		divHeight
	);

	mInstructionsButton.setPos(
		gScreenWidth / 2 - divWidth,
		7 * divHeight,
		divWidth * 2,
		divHeight
	);

	mSettingsButton.setPos(
		gScreenWidth / 2 - divWidth,
		9 * divHeight,
		divWidth * 2,
		divHeight
	);

	mExitButton.setPos(
		gScreenWidth / 2 - divWidth,
		11 * divHeight,
		divWidth * 2,
		divHeight
	);
}

void MenuState::Cleanup()
{
	mTitleImage.free();
}

void MenuState::Pause()
{

}

void MenuState::Resume()
{
	InitPos();
}

void MenuState::HandleEvents(GameEngine* game)
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

void MenuState::Update(GameEngine* game)
{
	if (mOnePlayerButton.clicked())
	{
		gBoard.setComputerPlaying(true);
		game->PushState(BoardSizeMenuState::Instance());
	}

	if (mTwoPlayerButton.clicked())
	{
		gBoard.setComputerPlaying(false);
		game->PushState(BoardSizeMenuState::Instance());
	}

	if (mInstructionsButton.clicked())
	{
		game->PushState(InstructionsState::Instance());
	}

	if (mSettingsButton.clicked())
	{
		game->PushState(SettingsState::Instance());
	}

	if (mExitButton.clicked())
	{
		game->Quit();
	}
}

void MenuState::Draw(GameEngine* game)
{
	int screenWidth = game->getScreenWidth();
	int screenHeight = game->getScreenHeight();

	int divHeight = screenHeight / 13;
	int divWidth = screenWidth / 6;

	mTitleImage.render(
		screenWidth / 2 - divWidth,
		divHeight,
		divWidth * 2,
		divHeight
	);

	mOnePlayerButton.render();

	mTwoPlayerButton.render();

	mInstructionsButton.render();

	mSettingsButton.render();

	mExitButton.render();
}
