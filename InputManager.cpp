#include "InputManager.h"

#include <Windows.h>

InputManager* InputManager::_pStatic_instance = nullptr;

/*
*		InputManager의 특수 맴버 함수
*/
InputManager::~InputManager()
{
}

/*
*		static 함수
*/
InputManager* InputManager::GetInstance()
{
	return _pStatic_instance;
}

void InputManager::Create()
{
	if (_pStatic_instance != nullptr) {
		return;
	}

	_pStatic_instance = new InputManager();
}

void InputManager::Destroy()
{
	if (_pStatic_instance == nullptr) {
		return;
	}

	delete _pStatic_instance;
}

void InputManager::SetInput(int keyCode)
{
	_keyInput[keyCode]._keyDown = true;
}

bool InputManager::MouseLeftClicked() const
{
	return _keyInput[VK_LBUTTON]._keyDown && !_keyInput[VK_LBUTTON]._prevKeyDown;
}

bool InputManager::MouseMiddleClicked() const
{
	return _keyInput[VK_MBUTTON]._keyDown && !_keyInput[VK_MBUTTON]._prevKeyDown;
}

bool InputManager::MouseRightClicked() const
{
	return _keyInput[VK_RBUTTON]._keyDown && !_keyInput[VK_RBUTTON]._prevKeyDown;
}

bool InputManager::MouseLeftPressed() const
{
	return _keyInput[VK_LBUTTON]._keyDown && _keyInput[VK_LBUTTON]._prevKeyDown;
}

bool InputManager::MouseMiddlePressed() const
{
	return _keyInput[VK_MBUTTON]._keyDown && _keyInput[VK_MBUTTON]._prevKeyDown;
}

bool InputManager::MouseRightPressed() const
{
	return _keyInput[VK_RBUTTON]._keyDown && _keyInput[VK_RBUTTON]._prevKeyDown;
}

bool InputManager::MouseLeftUp() const
{
	return !_keyInput[VK_LBUTTON]._keyDown && _keyInput[VK_LBUTTON]._prevKeyDown;
}

bool InputManager::MouseMiddleUp() const
{
	return !_keyInput[VK_MBUTTON]._keyDown && _keyInput[VK_MBUTTON]._prevKeyDown;
}

bool InputManager::MouseRightUp() const
{
	return !_keyInput[VK_RBUTTON]._keyDown && _keyInput[VK_RBUTTON]._prevKeyDown;
}

void InputManager::UpdateInput()
{
	for (int i = 0; i < 255; ++i)
	{
		_keyInput[i]._prevKeyDown = _keyInput[i]._keyDown;
		_keyInput[i]._keyDown = false;
	}
}
