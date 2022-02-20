#include "MainGame.h"
#include "Errors.h"
#include <iostream>

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
	gameLoop();
}

void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	// stops flickering :)
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	mWindow.create("Solitar", mScreenWidth, mScreenHeight, 0);

	// get the screen surface
	// mScreenSurface = mWindow.getScreenSurface();

	//get the renderer
	mRenderer = mWindow.getRenderer();


	//////////////// TEMP STUFF ////////////////////

	//m_SurfaceImg = loadSurface("textures/buton.png");

	//if (m_SurfaceImg == NULL)
	//{
	//	fatalError("Unable to load image!\n");
	//}

	//if (loadMedia("textures/buton.png") == false)
	//{
	//	std::cout << "Coulnd't load image";
	//}

	mFont = TTF_OpenFont("fonts/Roboto-Medium.ttf", 28);
	if (mFont == nullptr)
		fatalError("Font couldn't be opened");


	buton = LTexture(mRenderer);
	buton.loadFromFile("textures/buton.png");
	buton.setBlendMode(SDL_BLENDMODE_BLEND);

	mTextTexture = LTexture(mRenderer, mFont);

	SDL_Color color{ 0,0,0, 255 };
	mTextTexture.loadFromRenderedText("best engine", color);

}

void MainGame::gameLoop()
{
	while (mGameState == GameState::PLAY) {
		processInput();
		drawGame();
	}

	//Free loaded images
	mTextTexture.free();

	//Free global font
	//TTF_CloseFont(gFont);
	//gFont = NULL;


	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void MainGame::processInput()
{
	//Ask SDL to poll our events
	SDL_Event evnt{};

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			mGameState = GameState::EXIT;
			break;
		case SDL_MOUSEBUTTONUP:
			//_inputManager.releaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			//_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		};
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.key.keysym.sym)
			{
				
			}
		}
	}
}

void MainGame::drawGame()
{
	//// draw the surface pic on screen
	//SDL_Rect stretchRect;
	//stretchRect.x = 0;
	//stretchRect.y = 0;
	//stretchRect.w = m_screenWidth;
	//stretchRect.h = m_screenHeight;
	//SDL_BlitScaled(m_SurfaceImg, NULL, m_ScreenSurface, &stretchRect);

	////Update the surface
	//m_window.update();


	//Clear screen
	SDL_RenderClear(mRenderer);

	//Render texture to screen
	//SDL_RenderCopy(m_Renderer, m_Texture, NULL, NULL);

	//Render red filled quad
	SDL_Rect fillRect = { mScreenWidth / 4, mScreenHeight / 4, mScreenWidth / 2, mScreenHeight / 2 };
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(mRenderer, &fillRect);

	//Render green outlined quad
	SDL_Rect outlineRect = { mScreenWidth / 6, mScreenHeight / 6, mScreenWidth * 2 / 3, mScreenHeight * 2 / 3 };
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(mRenderer, &outlineRect);

	//Draw blue horizontal line
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawLine(mRenderer, 0, mScreenHeight / 2, mScreenWidth, mScreenHeight / 2);

	//Draw vertical line of yellow dots
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0x00, 0xFF);
	for (int i = 0; i < mScreenHeight; i += 4)
	{
		SDL_RenderDrawPoint(mRenderer, mScreenWidth / 2, i);
	}

	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	
	//printf("%d, %d, %d, %d\n", r, g, b, a);
	//buton.setColor(r, g, b);
	//buton.setAlpha(a);
	buton.render(100, 100, 2.0f);

	mTextTexture.render(100, 100);

	//Update screen
	SDL_RenderPresent(mRenderer);

	// Always swap buffer
	mWindow.swapBuffer();
}
