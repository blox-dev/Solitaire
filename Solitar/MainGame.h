#pragma once
#include <SDL/SDL.h>
#include <IMAGE/SDL_image.h>
#include "Window.h"
#include "LTexture.h"
enum class GameState {
	PLAY,
	EXIT
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();

	void gameLoop();
	void processInput();
	void drawGame();

	//temp
	TTF_Font* mFont;
	LTexture buton;
	LTexture mTextTexture;

	int mScreenWidth = 800;
	int mScreenHeight = 600;

	//The window renderer
	SDL_Renderer* mRenderer = nullptr;

	Window mWindow;
	GameState mGameState = GameState::PLAY;
};

