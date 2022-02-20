#pragma once

#include "InputManager.h"

#include <SDL/SDL.h>
#include <TTF/SDL_ttf.h>

#include <string>
#include <vector>

constexpr int MIN_BOARD_SIZE = 4;
constexpr int MAX_BOARD_SIZE = 20;

extern int gScreenWidth;
extern int gScreenHeight;
extern int gBoardSize;

class Board;
extern Board gBoard;

extern SDL_Color gBoardColor;
extern SDL_Color gPieceColor;
extern InputManager gInputManager;
extern SDL_Renderer* gRenderer;
extern TTF_Font* gRobotoFont;


enum WindowFlags {
	INVISIBLE = 0x1,
	FULLSCREEN = 0x2,
	BORDERLESS = 0x4
};

class GameState;

class GameEngine
{
public:

	void Init(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
	void swapBuffer();
	int getScreenWidth() { return mScreenWidth; };
	int getScreenHeight() { return mScreenHeight; };
	//static Context* const getSharedContext() { return &mContext; };

	void Cleanup();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return mRunning; }
	void Quit() { mRunning = false; }

	SDL_Surface* screen;

private:

	//static Context mContext;

	SDL_Renderer* mRenderer = nullptr;
	TTF_Font* mRobotoFont = nullptr;
	//InputManager mInputManager;

	// the stack of states
	std::vector<GameState*> states;

	SDL_Window* mSDLWindow = nullptr;

	int mScreenWidth = 800, mScreenHeight = 600;

	bool mRunning = false;
};