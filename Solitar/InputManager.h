#pragma once

#include <unordered_map>
#include <SDL/SDL.h>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void update();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	void setMouseCoords(int x, int y);
	SDL_Point getMouseCoords() const { return _mouseCoords; };

	bool isMouseInBox(int x0, int y0, int width, int height);

	// returns true if the key is held down
	bool isKeyDown(unsigned int keyID);

	// Returns true if the key was just release
	bool wasKeyReleased(unsigned int keyID);

	// Returns true if the key was just pressed
	bool wasKeyPressed(unsigned int keyID);
private:
	// returns true if the key was just released
	bool wasKeyDown(unsigned int keyID);

	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previousKeyMap;
	SDL_Point _mouseCoords;
};
