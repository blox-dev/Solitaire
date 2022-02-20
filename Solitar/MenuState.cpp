#include "MenuState.h"

#include "GameEngine.h"
#include "BoardSizeMenuState.h"

#include <SDL/SDL.h>

MenuState MenuState::mMenuState;

void MenuState::Init()
{
	printf("CMenuState Init\n");


	//title
	mTitleImage.loadFromFile("textures/Title.png");

	//buttons
	mButton.loadFromFile("textures/Button.png");
	mButtonHighlighted.loadFromFile("textures/ButtonHighlighted.png");
	mButtonPressed.loadFromFile("textures/ButtonPressed.png");

	SDL_Color color{255,255,255,255};

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
	
	mOnePlayerButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "1 Player");
	mTwoPlayerButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "2 Players");
	mInstructionsButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Instructions");
	mSettingsButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Settings");
	mExitButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Quit game");
}

void MenuState::Cleanup()
{
	printf("CMenuState Cleanup\n");
	mTitleImage.free();
	mButton.free();
	mButtonPressed.free();
	mButtonHighlighted.free();
}

void MenuState::Pause()
{
	printf("CMenuState Pause\n");
}

void MenuState::Resume()
{
	printf("CMenuState Resume\n");
}

void MenuState::HandleEvents(GameEngine* game)
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

	if (mOnePlayerButton.clicked())
	{
		printf("Clicked one player\n");
		game->PushState(BoardSizeMenuState::Instance());
	}
	if (mTwoPlayerButton.clicked())
	{
		printf("Clicked two players\n");
	}
	if (mInstructionsButton.clicked())
	{
		printf("Clicked instructions\n");
	}
	if (mSettingsButton.clicked())
	{
		printf("Clicked settings\n");
	}
	if (mExitButton.clicked())
	{
		printf("Clicked exit\n");
	}

}

void MenuState::Update(GameEngine* game)
{

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