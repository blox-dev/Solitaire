#include "GameEngine.h"

#include "LTexture.h"
#include "GameState.h"
#include "Board.h"
#include "Errors.h"

#include <SDL/SDL.h>
#include <IMAGE/SDL_image.h>
#include <TTF/SDL_ttf.h>

#include <iostream>

//Context GameEngine::mContext;
int gScreenWidth;
int gScreenHeight;
int gBoardSize;

// the board
Board gBoard = Board();

SDL_Color gBoardColor;
SDL_Color gPieceColor;
InputManager gInputManager;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gRobotoFont = nullptr;

void GameEngine::Init(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
{
	printf("CGameEngine Init\n");

	mRunning = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << SDL_GetError() << std::endl;
		fatalError("SDL Init failed :(");
	}

	// stops flickering :)
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	if (currentFlags & INVISIBLE)
		flags |= SDL_WINDOW_HIDDEN;

	if (currentFlags & FULLSCREEN)
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	if (currentFlags & BORDERLESS)
		flags |= SDL_WINDOW_BORDERLESS;

	mSDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

	if (mSDLWindow == nullptr) {
		fatalError("SDW Window could not be created!");
	}

	// set screen vars
	mScreenHeight = screenHeight;
	mScreenWidth = screenWidth;

	mRenderer = SDL_CreateRenderer(mSDLWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mRenderer == NULL)
	{
		fatalError("Renderer could not be created! SDL Error\n");
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;

	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << IMG_GetError();
		fatalError("SDL_image could not initialize!");
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		fatalError("SDL_ttf could not initialize!");
	}

	mRobotoFont = TTF_OpenFont("fonts/Roboto-Medium.ttf", 28);
	if (mRobotoFont == nullptr)
		fatalError("Font couldn't be opened");

	// setting context values

	//printf("%f",mInputManager.getMouseCoords().x);

	gRenderer = mRenderer;
	gRobotoFont = mRobotoFont;
	//gInputManager = &mInputManager;
	gScreenWidth = screenWidth;
	gScreenHeight = screenHeight;
	gBoardSize = 0;
	gBoardColor = SDL_Color{ 0,0,0,255 };
	gPieceColor = SDL_Color{ 255,255,255,255 };


	//LTexture::setSharedContext(&mContext);
	//GameState::setSharedContext(&mContext);

	// get the screen surface
	// m_screenSurface = SDL_GetWindowSurface(m_SDLWindow);

	//if (m_screenSurface == nullptr) {
	//	fatalError("Screen surface could not be created!");
	//}
}

void GameEngine::swapBuffer()
{
	SDL_GL_SwapWindow(mSDLWindow);
}

void GameEngine::Cleanup()
{
	// cleanup the all states
	while (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	printf("CGameEngine Cleanup\n");

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mSDLWindow);
	TTF_CloseFont(mRobotoFont);

	mRenderer = nullptr;
	mSDLWindow = nullptr;
	mRobotoFont = nullptr;

	// shutdown SDL systems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void GameEngine::ChangeState(GameState* state)
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void GameEngine::PushState(GameState* state)
{
	// pause current state
	if (!states.empty()) {
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void GameEngine::PopState()
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty()) {
		states.back()->Resume();
	}
}

void GameEngine::HandleEvents()
{
	// let the state handle events
	states.back()->HandleEvents(this);
}

void GameEngine::Update()
{
	//update the inputManager
	gInputManager.update();
	// let the state update the game
	states.back()->Update(this);
}

void GameEngine::Draw()
{
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
	// let the state draw the screen
	states.back()->Draw(this);

	SDL_RenderPresent(mRenderer);
	
	//swap the buffers
	swapBuffer();
}