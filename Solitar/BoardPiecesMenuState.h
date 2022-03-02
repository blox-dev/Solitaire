#pragma once

#include "GameEngine.h"
#include "GameState.h"
#include "LTexture.h"
#include "Button.h"

class BoardPiecesMenuState : public GameState
{
public:
	void Init() override;
	void Cleanup() override;

	void Pause() override;
	void Resume() override;

	void HandleEvents(GameEngine* game) override;
	void Update(GameEngine* game) override;
	void Draw(GameEngine* game) override;

	static BoardPiecesMenuState* Instance() {
		return &mBoardPiecesMenuState;
	}

protected:
	BoardPiecesMenuState() { }

private:
	// instance
	static BoardPiecesMenuState mBoardPiecesMenuState;

	//buttons

	Button mBackButton;
	Button mFillButton;
	Button mEmptyButton;
	Button mContinueButton;

	//text stuff
	LTexture infoText;
};