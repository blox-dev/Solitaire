#include "Slider.h"
#include <glm/glm.hpp>
#include <cmath>

Slider::Slider()
{
}

Slider::Slider(int min, int max, LTexture* sliderTexture, LTexture* notchTexture)
{
	mActive = true;

	minValue = min;
	maxValue = max;
	mValue = min;

	sTexture = sliderTexture;
	sNotchTexture = notchTexture;
}

Slider::~Slider()
{
}

void Slider::render()
{
	if (mActive)
	{
		if (sTexture != nullptr)
			sTexture->render(x, y, sWidth, sHeight);
		else {
			//draw simple grey rectangle
			SDL_Rect slider = { x, y + sHeight * 0.4, sWidth, sHeight * 0.2 };

			SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 255);
			SDL_RenderFillRect(gRenderer, &slider);
		}

		// notch center
		int centerX = ((float)(mValue - minValue) / (maxValue - minValue)) * sWidth + x;

		if (sNotchTexture != nullptr)
			sNotchTexture->render(centerX - 5, y, 10, sHeight);
		else {
			//with lighter grey notch on it
			SDL_Rect notch = { centerX - 5, y, 10, sHeight };

			SDL_SetRenderDrawColor(gRenderer, 196, 196, 196, 255);
			SDL_RenderFillRect(gRenderer, &notch);

		}
	}
}

void Slider::update()
{
	//on slider touch
	if (mActive && gInputManager.isMouseInBox(x, y, sWidth, sHeight) && gInputManager.wasKeyPressed(SDL_BUTTON_LEFT))
	{
		printf("slider pressed woo\n");
		mDragged = true;
	}
	// on slider drop
	if (mActive && gInputManager.wasKeyReleased(SDL_BUTTON_LEFT))
	{
		printf("slider maybe released :(\n");
		mDragged = false;
	}
	//if active
	if (mDragged)
	{
		glm::vec2 mouseCoords = gInputManager.getMouseCoords();
		if (mouseCoords.x <= x)
			mValue = minValue;
		else if (mouseCoords.x >= x + sWidth)
			mValue = maxValue;
		else
		{
			// (mousex - x) / sWidth = (mValue - minValue) / (maxValue - minValue)
			mValue = ((float)(mouseCoords.x - x) / sWidth) * (maxValue - minValue) + minValue;
		}
	}
}

void Slider::init(int min, int max, LTexture* sliderTexture, LTexture* notchTexture)
{
	mActive = true;
	
	minValue = min;
	maxValue = max;
	mValue = min;

	sTexture = sliderTexture;
	sNotchTexture = notchTexture;
}
