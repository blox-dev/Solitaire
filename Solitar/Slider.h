#pragma once

#include "LTexture.h"

class Slider
{
public:
	Slider();
	Slider(int min, int max, LTexture* sliderTexture = nullptr, LTexture* notchTexture = nullptr);
	~Slider();

	void render();
	void update();

	void setPos(int x0, int y0, int width, int height) { x = x0; y = y0; sWidth = width; sHeight = height; };

	void init(int min, int max, LTexture* sliderTexture = nullptr, LTexture* notchTexture = nullptr);


	int getValue() { return mValue; }
	void setValue(int newValue) { mValue = newValue; }

private:
	bool mActive = true;
	bool mDragged = false;

	int mValue = 0;
	int minValue = 0;
	int maxValue = 0;


	int x = 0, y = 0, sWidth = 0, sHeight = 0;

	LTexture* sTexture = nullptr;
	LTexture* sNotchTexture = nullptr;
};

