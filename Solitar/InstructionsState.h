#pragma once
#include <string.h>

#include "GameEngine.h"
#include "GameState.h"
#include "LTexture.h"
#include "Button.h"

class InstructionsState : public GameState
{
public:
	void Init() override;
	void Cleanup() override;

	void Pause() override;
	void Resume() override;

	void HandleEvents(GameEngine* game) override;
	void Update(GameEngine* game) override;
	void Draw(GameEngine* game) override;

	static InstructionsState* Instance() {
		return &mInstructionsState;
	}

protected:
	InstructionsState() { }

private:
	int TEXT_WIDTH = 0;
	int TEXT_HEIGHT = 0;
	// instance
	static InstructionsState mInstructionsState;

	const int ROW_PADDING = 2;

	//anything more than this looks bad
	LTexture textTexture[20];
	int textureNum;

	std::string instructionsText;
	
	Button mBackButton;
};

