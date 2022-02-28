#pragma once

#include "GameEngine.h"
#include "GameState.h"
#include "LTexture.h"
#include "Button.h"

class MenuState : public GameState
{
public:
	void Init() override;
	void Cleanup() override;

	void Pause() override;
	void Resume() override;

	void HandleEvents(GameEngine* game) override;
	void Update(GameEngine* game) override;
	void Draw(GameEngine* game) override;

	static MenuState* Instance() {
		return &mMenuState;
	}

protected:
	MenuState() { }

private:
	// instance
	static MenuState mMenuState;

	// drawing stuff
	LTexture mButton;
	LTexture mButtonPressed;
	LTexture mButtonHighlighted;
	//LTexture mBlockedButton;

	LTexture mTitleImage;

	Button mOnePlayerButton;
	Button mTwoPlayerButton;
	Button mInstructionsButton;
	Button mSettingsButton;
	Button mExitButton;
};
