#pragma once

#include "GameEngine.h"
#include "GameState.h"
#include "LTexture.h"
#include "Button.h"

class SettingsState : public GameState
{
public:
	void Init() override;
	void Cleanup() override;

	void Pause() override;
	void Resume() override;

	void HandleEvents(GameEngine* game) override;
	void Update(GameEngine* game) override;
	void Draw(GameEngine* game) override;

	static SettingsState* Instance() {
		return &mSettingsState;
	}

protected:
	SettingsState() { }

private:
	void hexStringToSDLColor(SDL_Color& color, std::string hex);
	const static int COLORS_SIZE = 5;

	// instance
	static SettingsState mSettingsState;

	//buttons
	Button mBoardColorLeftArrow;
	Button mBoardColorRightArrow;
	Button mPieceColorLeftArrow;
	Button mPieceColorRightArrow;
	Button mBackButton;

	//text
	LTexture mBoardColorText;
	LTexture mPieceColorText;

	//colors
	SDL_Color boardColors[COLORS_SIZE];
	SDL_Color pieceColors[COLORS_SIZE];
	int boardColorIndex = 0;
	int pieceColorIndex = 0;
};
