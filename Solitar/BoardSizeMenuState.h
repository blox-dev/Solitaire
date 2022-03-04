#pragma once

#include "GameEngine.h"
#include "GameState.h"
#include "LTexture.h"
#include "Button.h"
#include "Slider.h"

class BoardSizeMenuState : public GameState
{
public:
	void Init() override;
	void Cleanup() override;

	void Pause() override;
	void Resume() override;

	void HandleEvents(GameEngine* game) override;
	void Update(GameEngine* game) override;
	void Draw(GameEngine* game) override;

	static BoardSizeMenuState* Instance() {
		return &mBoardSizeMenuState;
	}

protected:
	BoardSizeMenuState() { }

private:
	// instance
	static BoardSizeMenuState mBoardSizeMenuState;

	// buttons

	Button mBackButton;
	Button mContinueButton;
	Slider mValueSlider;

	//text stuff
	LTexture infoText;
};