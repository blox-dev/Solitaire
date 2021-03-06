#pragma once

#include "GameEngine.h"
#include "GameState.h"
#include "LTexture.h"
#include "Button.h"

class MenuState : public GameState
{
private:
	void InitPos();
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

	// title
	LTexture mTitleImage;

	// buttons
	Button mOnePlayerButton;
	Button mTwoPlayerButton;
	Button mInstructionsButton;
	Button mSettingsButton;
	Button mExitButton;
};
