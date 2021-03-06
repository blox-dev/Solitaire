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
	//colors

	//black
	hexStringToSDLColor(boardColors[0], "#000000");
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

	//buttons

	mBoardColorLeftArrow.init(
		"",
		gCommonTextures.leftArrow,
		gCommonTextures.leftArrowPressed,
		gCommonTextures.leftArrowHighlighted,
		nullptr
	);

	mPieceColorLeftArrow.init(
		"",
		gCommonTextures.leftArrow,
		gCommonTextures.leftArrowPressed,
		gCommonTextures.leftArrowHighlighted,
		nullptr
	);

	mBoardColorRightArrow.init(
		"",
		gCommonTextures.rightArrow,
		gCommonTextures.rightArrowPressed,
		gCommonTextures.rightArrowHighlighted,
		nullptr
	);

	mPieceColorRightArrow.init(
		"",
		gCommonTextures.rightArrow,
		gCommonTextures.rightArrowPressed,
		gCommonTextures.rightArrowHighlighted,
		nullptr
	);

	mBackButton.init(
		"Back",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	//text

	mPieceColorText.loadFromRenderedText("Piece color ");
	mBoardColorText.loadFromRenderedText("Board color ");

	InitPos();
}

void SettingsState::InitPos()
{
	//set positions
	const int HEIGHT_PADDING = 20;
	const int WIDTH_PADDING = 5;

	mBoardColorLeftArrow.setPos(
		WIDTH_PADDING + mBoardColorText.tWidth,
		HEIGHT_PADDING,
		mBoardColorText.tHeight,
		mBoardColorText.tHeight
	);

	mBoardColorRightArrow.setPos(
		WIDTH_PADDING * 3 + mBoardColorText.tWidth + mBoardColorText.tHeight * 2,
		HEIGHT_PADDING,
		mBoardColorText.tHeight,
		mBoardColorText.tHeight
	);

	mPieceColorLeftArrow.setPos(
		WIDTH_PADDING + mPieceColorText.tWidth,
		HEIGHT_PADDING * 2 + mBoardColorText.tHeight,
		mPieceColorText.tHeight,
		mPieceColorText.tHeight
	);

	mPieceColorRightArrow.setPos(
		WIDTH_PADDING * 3 + mPieceColorText.tWidth + 2 * mPieceColorText.tHeight,
		HEIGHT_PADDING * 2 + mBoardColorText.tHeight,
		mPieceColorText.tHeight,
		mPieceColorText.tHeight
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
	mBoardColorText.free();
	mPieceColorText.free();
}

void SettingsState::Pause()
{

}

void SettingsState::Resume()
{
	InitPos();
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

void SettingsState::Update(GameEngine* game)
{
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

void SettingsState::Draw(GameEngine* game)
{
	static const int HEIGHT_PADDING = 20;
	static const int WIDTH_PADDING = 5;

	int screenWidth = game->getScreenWidth();
	int screenHeight = game->getScreenHeight();

	// board color stuff

	mBoardColorText.render(WIDTH_PADDING, HEIGHT_PADDING, 1.0f);

	mBoardColorLeftArrow.render();
	mBoardColorRightArrow.render();

	SDL_Rect boardColorRect = {
		WIDTH_PADDING * 2 + mBoardColorText.tWidth + mBoardColorText.tHeight,
		HEIGHT_PADDING,
		mBoardColorText.tHeight,
		mBoardColorText.tHeight
	};

	SDL_SetRenderDrawColor(gRenderer, gBoardColor.r, gBoardColor.g, gBoardColor.b, gBoardColor.a);
	SDL_RenderFillRect(gRenderer, &boardColorRect);

	//white outline
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &boardColorRect);

	// piece color stuff

	mPieceColorText.render(WIDTH_PADDING, HEIGHT_PADDING * 2 + mBoardColorText.tHeight, 1.0f);

	mPieceColorLeftArrow.render();
	mPieceColorRightArrow.render();

	SDL_Rect pieceColorRect = {
		WIDTH_PADDING * 2 + mPieceColorText.tWidth + mPieceColorText.tHeight,
		HEIGHT_PADDING * 2 + mBoardColorText.tHeight,
		mPieceColorText.tHeight,
		mPieceColorText.tHeight
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
