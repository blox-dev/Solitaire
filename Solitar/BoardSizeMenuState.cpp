#include "BoardSizeMenuState.h"
#include "Board.h"
#include "GameEngine.h"

#include <SDL/SDL.h>
#include "BoardShapeMenuState.h"

BoardSizeMenuState BoardSizeMenuState::mBoardSizeMenuState;

void BoardSizeMenuState::Init()
{
	printf("BoardSizeMenuState Init\n");


	infoText.loadFromRenderedText("Type the desired board size (" + std::to_string(MIN_BOARD_SIZE) + " <= n <= " + std::to_string(MAX_BOARD_SIZE) + ")");
	nText.loadFromRenderedText("n=");
	inputTextTexture.loadFromRenderedText("");

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
	
	// blocked until the boardSize is in range
	mContinueButton.setBlocked(true);

	mBackButton.setPos(
		gScreenWidth * 0.05,
		gScreenHeight * 0.85,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);

	gBoard.setBoardSize(1);

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
	nText.free();
	inputTextTexture.free();
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
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
			{
				//remove last character
				inputText.pop_back();
				renderText = true;

				if (inputText.empty())
					mContinueButton.setBlocked(true);
				else if (stoi(inputText) < MIN_BOARD_SIZE || stoi(inputText) > MAX_BOARD_SIZE)
					mContinueButton.setBlocked(true);
				else mContinueButton.setBlocked(false);
			}
			else if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)
			{
				
				inputText += event.key.keysym.sym;

				if (stoi(inputText) > MAX_BOARD_SIZE)
					inputText.pop_back();

				if (inputText.empty())
					mContinueButton.setBlocked(true);
				else if (stoi(inputText) >= MIN_BOARD_SIZE && stoi(inputText) <= MAX_BOARD_SIZE)
					mContinueButton.setBlocked(false);
				else mContinueButton.setBlocked(true);

				renderText = true;
			}
			break;

		case SDL_QUIT:
			game->Quit();
			break;
		}
	}

	if (renderText) {
		// bad implementation probably
		renderText = false;
		if (inputText.empty())
			inputTextTexture.loadFromRenderedText(" ", SDL_Color{255,255,255,255});
		else
			inputTextTexture.loadFromRenderedText(inputText, SDL_Color{ 255,255,255,255 });
	
		if (inputText.empty())
			gBoardSize = 1;
		else gBoardSize = stoi(inputText);

		gBoard.setBoardSize(gBoardSize);
	}

	if (mContinueButton.clicked())
	{
		gBoardSize = stoi(inputText);
		gBoard.setBoardSize(gBoardSize);
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
	
	nText.render(5, 5 + infoText.tHeight, 1.0f);

	inputTextTexture.render(5 + nText.tWidth, 5 + infoText.tHeight, 1.0f);

	gBoard.draw(BoardMode::EMPTY);

	mContinueButton.render();
	mBackButton.render();
}
