#pragma once

#include "GameEngine.h"
#include "GameState.h"
#include "LTexture.h"
#include "Button.h"

class BoardShapeMenuState : public GameState
{
public:
	void Init() override;
	void Cleanup() override;

	void Pause() override;
	void Resume() override;

	void HandleEvents(GameEngine* game) override;
	void Update(GameEngine* game) override;
	void Draw(GameEngine* game) override;

	static BoardShapeMenuState* Instance() {
		return &mBoardShapeMenuState;
	}

protected:
	BoardShapeMenuState() { }

private:
	// instance
	static BoardShapeMenuState mBoardShapeMenuState;

	// drawing stuff
	LTexture mButton;
	LTexture mButtonPressed;
	LTexture mButtonHighlighted;
	LTexture mButtonBlocked;

	Button mBackButton;
	Button mContinueButton;

	//text stuff
	LTexture infoText;
	LTexture nText;

	LTexture inputTextTexture;
};