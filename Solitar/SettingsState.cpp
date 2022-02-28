#include "SettingsState.h"

#include "GameEngine.h"
#include "BoardSizeMenuState.h"
#include "Board.h"

#include <SDL/SDL.h>
#include "InstructionsState.h"
#include <regex>

SettingsState SettingsState::mSettingsState;

void SettingsState::Init()
{
	printf("SettingsState Init\n");

	//colors

	//black
	hexStringToSDLColor(boardColors[0],"#000000");
	//green
	hexStringToSDLColor(boardColors[1], "#065535");
	//grey
	hexStringToSDLColor(boardColors[2], "#666666");
	//blue
	hexStringToSDLColor(boardColors[3], "#003366");
	//crimson red?
	hexStringToSDLColor(boardColors[4], "#800000");
	
	//white
	hexStringToSDLColor(pieceColors[0], "#ffffff");
	//light yellow
	hexStringToSDLColor(pieceColors[1], "#fff68f");
	//turqoise
	hexStringToSDLColor(pieceColors[2], "#afeeee");
	//light blue
	hexStringToSDLColor(pieceColors[3], "#3399ff");
	//light green
	hexStringToSDLColor(pieceColors[4], "#b4eeb4");

	//button textures
	mButton.loadFromFile("textures/Button.png");
	mButtonHighlighted.loadFromFile("textures/ButtonHighlighted.png");
	mButtonPressed.loadFromFile("textures/ButtonPressed.png");

	mLeftArrowButtonTexture.loadFromFile("textures/Left.png");
	mLeftArrowButtonHighlightedTexture.loadFromFile("textures/LeftHighlighted.png");
	mLeftArrowButtonPressedTexture.loadFromFile("textures/LeftPressed.png");

	mRightArrowButtonTexture.loadFromFile("textures/Right.png");
	mRightArrowButtonHighlightedTexture.loadFromFile("textures/RightHighlighted.png");
	mRightArrowButtonPressedTexture.loadFromFile("textures/RightPressed.png");


	//buttons

	mBoardColorLeftArrow.init(&mLeftArrowButtonTexture, &mLeftArrowButtonPressedTexture, &mLeftArrowButtonHighlightedTexture, "");
	mPieceColorLeftArrow.init(&mLeftArrowButtonTexture, &mLeftArrowButtonPressedTexture, &mLeftArrowButtonHighlightedTexture, "");

	mBoardColorRightArrow.init(&mRightArrowButtonTexture, &mRightArrowButtonPressedTexture, &mRightArrowButtonHighlightedTexture, "");
	mPieceColorRightArrow.init(&mRightArrowButtonTexture, &mRightArrowButtonPressedTexture, &mRightArrowButtonHighlightedTexture, "");
	
	mBackButton.init(&mButton, &mButtonPressed, &mButtonHighlighted, "Back");

	//text

	SDL_Color color{ 255,255,255,255 };
	mPieceColorText.loadFromRenderedText("Piece color:", color);
	mBoardColorText.loadFromRenderedText("Board color:", color);
	
	//set positions

	mBoardColorLeftArrow.setPos(
		0.3 * gScreenWidth,
		0.1 * gScreenHeight,
		0.1 * gScreenWidth,
		0.1 * gScreenHeight
	);

	mBoardColorRightArrow.setPos(
		0.52*gScreenWidth,
		0.1*gScreenHeight,
		0.1*gScreenWidth,
		0.1*gScreenHeight
	);

	mPieceColorLeftArrow.setPos(
		0.3 * gScreenWidth,
		0.25 * gScreenHeight,
		0.1 * gScreenWidth,
		0.1 * gScreenHeight
	);

	mPieceColorRightArrow.setPos(
		0.52 * gScreenWidth,
		0.25 * gScreenHeight,
		0.1 * gScreenWidth,
		0.1 * gScreenHeight
	);

	mBackButton.setPos(
		0.1 * gScreenWidth,
		0.85 * gScreenHeight,
		0.1 * gScreenWidth,
		0.1 * gScreenHeight
	);
}

void SettingsState::Cleanup()
{
	printf("SettingsState Cleanup\n");

	mButton.free();
	mButtonPressed.free();
	mButtonHighlighted.free();

	mLeftArrowButtonTexture.free();
	mLeftArrowButtonPressedTexture.free();
	mLeftArrowButtonHighlightedTexture.free();

	mRightArrowButtonTexture.free();
	mRightArrowButtonPressedTexture.free();
	mRightArrowButtonHighlightedTexture.free();
}

void SettingsState::Pause()
{
	printf("SettingsState Pause\n");
}

void SettingsState::Resume()
{
	printf("SettingsState Resume\n");
}

void SettingsState::HandleEvents(GameEngine* game)
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

	if (mBackButton.clicked())
	{
		game->PopState();
	}
	//change the board colors
	if (mBoardColorLeftArrow.clicked())
	{
		boardColorIndex--;
		if (boardColorIndex < 0)
			boardColorIndex = COLORS_SIZE - 1;
		gBoardColor = boardColors[boardColorIndex];
	}
	if (mBoardColorRightArrow.clicked())
	{
		boardColorIndex++;
		if (boardColorIndex >= COLORS_SIZE)
			boardColorIndex = 0;
		gBoardColor = boardColors[boardColorIndex];
	}
	//change the piece colors
	if (mPieceColorLeftArrow.clicked())
	{
		pieceColorIndex--;
		if (pieceColorIndex < 0)
			pieceColorIndex = COLORS_SIZE - 1;
		gPieceColor = pieceColors[pieceColorIndex];
	}
	if (mPieceColorRightArrow.clicked())
	{
		pieceColorIndex++;
		if (pieceColorIndex >= COLORS_SIZE)
			pieceColorIndex = 0;
		gPieceColor = pieceColors[pieceColorIndex];
	}
}

void SettingsState::Update(GameEngine* game)
{

}

void SettingsState::Draw(GameEngine* game)
{
	int screenWidth = game->getScreenWidth();
	int screenHeight = game->getScreenHeight();

	// board color stuff

	mBoardColorText.render(
		0.05 * gScreenWidth,
		0.1 * gScreenHeight,
		0.25 * gScreenWidth,
		0.1 * gScreenHeight
	);

	mBoardColorLeftArrow.render();
	mBoardColorRightArrow.render();

	SDL_Rect boardColorRect = { 
		0.41 * gScreenWidth,
		0.1 * gScreenHeight,
		0.1 * gScreenWidth,
		0.1 * gScreenHeight
	};
	SDL_SetRenderDrawColor(gRenderer, gBoardColor.r, gBoardColor.g, gBoardColor.b, gBoardColor.a);
	SDL_RenderFillRect(gRenderer, &boardColorRect);

	//white outline
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &boardColorRect);

	// piece color stuff

	mPieceColorText.render(
		0.05 * gScreenWidth,
		0.25 * gScreenHeight,
		0.25 * gScreenWidth,
		0.1 * gScreenHeight
	);

	mPieceColorLeftArrow.render();
	mPieceColorRightArrow.render();

	SDL_Rect pieceColorRect = {
		0.41 * gScreenWidth,
		0.25 * gScreenHeight,
		0.1 * gScreenWidth,
		0.1 * gScreenHeight
	};

	SDL_SetRenderDrawColor(gRenderer, gPieceColor.r, gPieceColor.g, gPieceColor.b, gPieceColor.a);
	SDL_RenderFillRect(gRenderer, &pieceColorRect);

	//white outline
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &pieceColorRect);

	//back button

	mBackButton.render();
}

void SettingsState::hexStringToSDLColor(SDL_Color& color, std::string hexString)
{
	std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

	std::smatch match;
	if (std::regex_match(hexString, match, pattern))
	{
		// From kraskevich's comment
		color.r = std::stoul(match[1].str(), nullptr, 16);
		color.g = std::stoul(match[2].str(), nullptr, 16);
		color.b = std::stoul(match[3].str(), nullptr, 16);
		color.a = 255;
	}
	else
	{
		printf("%s is an invalid rgb color\n", hexString.c_str());
	}
}
