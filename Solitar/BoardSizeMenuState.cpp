#include "BoardSizeMenuState.h"
#include "Board.h"
#include "GameEngine.h"

#include <SDL/SDL.h>
#include "BoardShapeMenuState.h"

BoardSizeMenuState BoardSizeMenuState::mBoardSizeMenuState;

void BoardSizeMenuState::Init()
{
	printf("BoardSizeMenuState Init\n");

	//buttons
	mButton.loadFromFile("textures/Button.png");
	mButtonHighlighted.loadFromFile("textures/ButtonHighlighted.png");
	mButtonPressed.loadFromFile("textures/ButtonPressed.png");
	mButtonBlocked.loadFromFile("textures/ButtonBlocked.png");

	SDL_Color color{ 255,255,255,255 };

	infoText.loadFromRenderedText("Type the desired board size (" + std::to_string(MIN_BOARD_SIZE) + " <= n <= " + std::to_string(MAX_BOARD_SIZE) + ")", color);
	nText.loadFromRenderedText("n=", color);
	inputTextTexture.loadFromRenderedText(" ", color);

	mContinueButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Continue");
	mBackButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Back");

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
}

void BoardSizeMenuState::Cleanup()
{
	printf("BoardSizeMenuState Cleanup\n");

	mButton.free();
	mButtonPressed.free();
	mButtonHighlighted.free();
	mButtonBlocked.free();

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
			}
			else if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)
			{
				
				inputText += event.key.keysym.sym;

				if (stoi(inputText) > MAX_BOARD_SIZE)
					inputText.pop_back();
				
				renderText = true;
			}
			break;

		case SDL_QUIT:
			game->Quit();
			break;
		}
	}

	if (renderText) {
		renderText = false;
		if(inputText.empty())
			inputTextTexture.loadFromRenderedText(" ", SDL_Color{255,255,255,255});
		else
			inputTextTexture.loadFromRenderedText(inputText, SDL_Color{ 255,255,255,255 });
	}

	if (mContinueButton.clicked())
	{
		gBoardSize = stoi(inputText);
		gBoard.setBoardSize(gBoardSize);

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
	int h = game->getScreenHeight();
	int w = game->getScreenWidth();
	SDL_Renderer* r = gRenderer;
	
	infoText.render(5, 5, w-10, 25);
	nText.render(5, 30, 25*2, 25);
	inputTextTexture.render(50, 30, 25*inputText.length(), 25);

	//Render red filled quad
	SDL_Rect fillRect = { w / 4, h / 4, w / 2, h / 2 };
	SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(r, &fillRect);

	//Render green outlined quad
	SDL_Rect outlineRect = { w / 6, h / 6, w * 2 / 3, h * 2 / 3 };
	SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(r, &outlineRect);

	//Draw blue horizontal line
	SDL_SetRenderDrawColor(r, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawLine(r, 0, h / 2, w, h / 2);


	mContinueButton.render();
	mBackButton.render();
}
