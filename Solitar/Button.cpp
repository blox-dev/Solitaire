#include "Button.h"
#include <iostream>
Button::Button()
{
}

void Button::init(LTexture* buttonTexture, LTexture* buttonPressedTexture, LTexture* buttonHighlightedTexture, std::string buttonText, SDL_Color color)
{
	bTexture = buttonTexture;
	bPressedTexture = buttonPressedTexture;
	bHighlightedTexture = buttonHighlightedTexture;
	bText.loadFromRenderedText(buttonText.c_str(), color);
}

void Button::render()
{
	//std::cout << mInputManager->getMouseCoords().x << ',' << mInputManager->getMouseCoords().y << std::endl;
	if (gInputManager.isMouseInBox(x, y, bWidth, bHeight))
	{
		if (gInputManager.isKeyDown(SDL_BUTTON_LEFT))
		{
			bPressedTexture->render(x, y, bWidth, bHeight);
			bText.render(x + bWidth / 4, y + bHeight / 3, bWidth / 2, bHeight / 2);
		}
		else {
			bHighlightedTexture->render(x, y, bWidth, bHeight);
			bText.render(x + bWidth / 4, y + bHeight / 4, bWidth / 2, bHeight / 2);
		}
	}
	else {
		bTexture->render(x, y, bWidth, bHeight);
		bText.render(x + bWidth / 4, y + bHeight / 4, bWidth / 2, bHeight / 2);
	}
}

bool Button::clicked()
{
	return gInputManager.isMouseInBox(x, y, bWidth, bHeight) && gInputManager.isKeyPressed(SDL_BUTTON_LEFT);
}
