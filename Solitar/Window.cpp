#include "Window.h"
#include "Errors.h"
#include <iostream>

Window::Window()
{
}

Window::~Window()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_SDLWindow);
}

int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
{
	Uint64 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	if (currentFlags & INVISIBLE)
		flags |= SDL_WINDOW_HIDDEN;

	if (currentFlags & FULLSCREEN)
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	if (currentFlags & BORDERLESS)
		flags |= SDL_WINDOW_BORDERLESS;

	m_SDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

	if (m_SDLWindow == nullptr) {
		fatalError("SDW Window could not be created!");
	}

	m_Renderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
	if (m_Renderer == NULL)
	{
		fatalError("Renderer could not be created! SDL Error\n");
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);

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

	// get the screen surface
	m_screenSurface = SDL_GetWindowSurface(m_SDLWindow);

	if (m_screenSurface == nullptr) {
		fatalError("Screen surface could not be created!");
	}

	//GLenum error = glewInit();
	//if (error != GLEW_OK) {
	//	fatalError("Could not init glew");
	//}

	//std::printf("*** OpenGL Version: %s ***", glGetString(GL_VERSION));

	// set the default background color
	//glClearColor(1, 1, 1, 1.0f);

	//Enable alpha blend
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// turn on VSync
	// SDL_GL_SetSwapInterval(1);

    return 0;
}

void Window::swapBuffer()
{
	SDL_GL_SwapWindow(m_SDLWindow);
}

void Window::update()
{
	//Update the surface
	SDL_UpdateWindowSurface(m_SDLWindow);
}


