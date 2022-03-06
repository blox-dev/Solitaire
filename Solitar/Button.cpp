#include "Button.h"
#include <iostream>
Button::Button()
{
}

Button::~Button()
{
	bText.free();
}

void Button::init(std::string buttonText, LTexture* buttonTexture, LTexture* buttonPressedTexture, LTexture* buttonHighlightedTexture, LTexture* buttonBlockedTexture, SDL_Color color)
{
	//maybe not necessary
	bActive = true;
	bBlocked = false;

	bTexture = buttonTexture;
	bPressedTexture = buttonPressedTexture;
	bHighlightedTexture = buttonHighlightedTexture;
	bBlockedTexture = buttonBlockedTexture;

	bText.loadFromRenderedText(buttonText.c_str(), color);
}

void Button::render()
{
	if (bActive) {
		if (bBlocked)
		{
			if (bBlockedTexture->isSet())
				bBlockedTexture->render(x, y, bWidth, bHeight);
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 255);
				SDL_RenderFillRect(gRenderer, &buttonRect);
			}

			bText.render(x + bWidth / 4, y + bHeight / 3, bWidth / 2, bHeight / 2);
			return;
		}
		if (gInputManager.isMouseInBox(x, y, bWidth, bHeight))
		{
			if (gInputManager.isKeyDown(SDL_BUTTON_LEFT))
			{
				if (bPressedTexture->isSet())
					bPressedTexture->render(x, y, bWidth, bHeight);
				else {
					SDL_SetRenderDrawColor(gRenderer, 122, 211, 83, 255);
					SDL_RenderFillRect(gRenderer, &buttonRect);
				}
				bText.render(x + bWidth / 4, y + bHeight / 3, bWidth / 2, bHeight / 2);
			}
			else {
				if (bHighlightedTexture->isSet())
					bHighlightedTexture->render(x, y, bWidth, bHeight);
				else {
					SDL_SetRenderDrawColor(gRenderer, 140, 225, 96, 255);
					SDL_RenderFillRect(gRenderer, &buttonRect);
				}
				bText.render(x + bWidth / 4, y + bHeight / 4, bWidth / 2, bHeight / 2);
			}
		}
		else {
			if (bTexture->isSet())
				bTexture->render(x, y, bWidth, bHeight);
			else {
				SDL_SetRenderDrawColor(gRenderer, 49, 143, 0, 255);
				SDL_RenderFillRect(gRenderer, &buttonRect);
			}
			bText.render(x + bWidth / 4, y + bHeight / 4, bWidth / 2, bHeight / 2);
		}
	}
}

bool Button::clicked()
{
	return !bBlocked && bActive && gInputManager.isMouseInBox(x, y, bWidth, bHeight) && gInputManager.wasKeyReleased(SDL_BUTTON_LEFT);
}
