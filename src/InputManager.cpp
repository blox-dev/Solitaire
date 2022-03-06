#include "InputManager.h"

InputManager::InputManager() : _mouseCoords({ 0,0 })
{
}

InputManager::~InputManager()
{
}

void InputManager::update()
{
	for (auto& it : _keyMap) {
		_previousKeyMap[it.first] = it.second;
	}
}
void InputManager::pressKey(unsigned int keyID)
{
	_keyMap[keyID] = true;
}
void InputManager::releaseKey(unsigned int keyID)
{
	_keyMap[keyID] = false;
}
void InputManager::setMouseCoords(int x, int y)
{
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}
bool InputManager::isMouseInBox(int x0, int y0, int width, int height)
{
	return (x0 <= _mouseCoords.x) && (_mouseCoords.x <= x0 + width) && (y0 <= _mouseCoords.y) && (_mouseCoords.y <= y0 + height);
}
bool InputManager::isKeyDown(unsigned int keyID)
{
	auto it = _keyMap.find(keyID);
	return (it == _keyMap.end()) ? false : it->second;
}

bool InputManager::wasKeyReleased(unsigned int keyID)
{
	return !isKeyDown(keyID) && wasKeyDown(keyID);
}

bool InputManager::wasKeyPressed(unsigned int keyID)
{
	return isKeyDown(keyID) && !wasKeyDown(keyID);
}

bool InputManager::wasKeyDown(unsigned int keyID)
{
	auto it = _previousKeyMap.find(keyID);
	return (it == _previousKeyMap.end()) ? false : it->second;
}
