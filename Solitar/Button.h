#pragma once

#include "LTexture.h"
#include "InputManager.h"

class Button : public LTexture
{
public:
	Button();
	~Button();

	void setPos(int x0, int y0, int width, int height) { x = x0; y = y0; bWidth = width; bHeight = height; };

	void init(
		LTexture* buttonTexture, LTexture* buttonPressedTexture, LTexture* buttonHighlightedTexture, LTexture* buttonBlockedTexture,
		std::string buttonText = " ", SDL_Color color = SDL_Color{ 255,255,255,255 }
	);

	void setActive(bool active) { bActive = active; }
	void setBlocked(bool blocked) { bBlocked = blocked; }

	void render();
	bool clicked();

protected:
	int x = 0, y = 0, bWidth = 0, bHeight = 0;
	bool bActive = true;
	bool bBlocked = false;

	LTexture bText;
	LTexture* bTexture = nullptr;
	LTexture* bPressedTexture = nullptr;
	LTexture* bHighlightedTexture = nullptr;
	LTexture* bBlockedTexture = nullptr;
};

