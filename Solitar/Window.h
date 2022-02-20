#pragma once
#include <string>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <IMAGE/SDL_image.h>
#include <TTF/SDL_ttf.h>

enum WindowFlags {
	INVISIBLE = 0x1,
	FULLSCREEN = 0x2,
	BORDERLESS = 0x4
};

class Window
{
public:
	Window();
	~Window();

	int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

	void swapBuffer();
	void update();

	int getScreenWidth() { return m_screenWidth; };
	int getScreenHeight() { return m_screenHeight; };

	SDL_Surface* getScreenSurface() { return m_screenSurface; };
	SDL_Renderer* getRenderer() { return m_Renderer; };
private:
	SDL_Renderer* m_Renderer = nullptr;
	SDL_Window* m_SDLWindow = nullptr;
	SDL_Surface* m_screenSurface = nullptr;
	int m_screenWidth = 800, m_screenHeight = 600;
};

