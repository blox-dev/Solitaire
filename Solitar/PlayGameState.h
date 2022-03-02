#pragma once

#include "GameEngine.h"
#include "GameState.h"
#include "LTexture.h"
#include "Button.h"

class PlayGameState : public GameState
{
public:
	void Init() override;
	void Cleanup() override;

	void Pause() override;
	void Resume() override;

	void HandleEvents(GameEngine* game) override;
	void Update(GameEngine* game) override;
	void Draw(GameEngine* game) override;

	static PlayGameState* Instance() {
		return &mPlayGameState;
	}

protected:
	PlayGameState() { }

private:
	// instance
	static PlayGameState mPlayGameState;

	int mPlayerTurn = 1;

	// buttons
	Button mBackButton;

	//text stuff
	LTexture mPlayer1Text;
	LTexture mPlayer2Text;
	LTexture mComputerText;

	LTexture mDrawText;
	LTexture mPlayer1WinText;
	LTexture mPlayer2WinText;
	LTexture mComputerWinText;

	LTexture mScoreText;
};

