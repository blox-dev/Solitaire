#include "InstructionsState.h"

#include "GameEngine.h"
#include "BoardSizeMenuState.h"
#include "Board.h"

#include <SDL/SDL.h>

InstructionsState InstructionsState::mInstructionsState;

void InstructionsState::InitPos()
{
	mBackButton.setPos(
		gScreenWidth * 0.05,
		gScreenHeight * 0.85,
		gScreenWidth * 0.15,
		gScreenHeight * 0.1
	);
}

void InstructionsState::Init()
{
	textureNum = 0;

	mBackButton.init(
		"Back",
		gCommonTextures.button,
		gCommonTextures.buttonPressed,
		gCommonTextures.buttonHighlighted,
		gCommonTextures.buttonBlocked
	);

	InitPos();

	instructionsText =
		"Solitaire can be played by two players or by one player and the computer. \
		A valid move is to jump a piece orthogonally over an adjacent piece into an empty hole two positions away and then to remove the jumped piece. \
		Once there are no valid moves left, the player with the highest score wins!";

	TTF_SizeText(gRobotoFont, instructionsText.c_str(), &TEXT_WIDTH, &TEXT_HEIGHT);

	TEXT_WIDTH /= instructionsText.length();

	//parse the instructions text
	int chars_per_line = (gScreenWidth - 10) / TEXT_WIDTH;

	size_t last = 0;
	size_t next = 0;

	std::string current_string("");

	int current_char_count = 0;
	while ((next = instructionsText.find(" ", last)) != std::string::npos) {
		if ((next - last + 1) + current_char_count < chars_per_line)
		{
			current_char_count += (next - last + 1);
			current_string += instructionsText.substr(last, next - last) + " ";
		}
		else
		{
			textTexture[textureNum++].loadFromRenderedText(current_string);

			current_string = instructionsText.substr(last, next - last) + " ";
			current_char_count = current_string.length();
		}
		last = next + 1;
	}
	textTexture[textureNum++].loadFromRenderedText(current_string + instructionsText.substr(last));
}

void InstructionsState::Cleanup()
{

}

void InstructionsState::Pause()
{

}

void InstructionsState::Resume()
{
	InitPos();
}

void InstructionsState::HandleEvents(GameEngine* game)
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

void InstructionsState::Update(GameEngine* game)
{
	if (mBackButton.clicked())
	{
		game->PopState();
	}
}

void InstructionsState::Draw(GameEngine* game)
{
	//draw the instructions text
	for (int i = 0; i < textureNum; ++i)
	{
		textTexture[i].render(5, 5 + i * (TEXT_HEIGHT + ROW_PADDING), 1.0f);
	}

	mBackButton.render();
}
