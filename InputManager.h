#pragma once

struct KeyInput
{
	bool _keyDown = false;
	bool _prevKeyDown = false;
};

class InputManager
{
private:
	InputManager() = default;
	~InputManager();
public:
	InputManager(const InputManager& rhs)				= delete;
	InputManager(InputManager&& rhs)					= delete;
	InputManager& operator=(const InputManager& rhs)	= delete;
	InputManager& operator=(InputManager&& rhs)			= delete;

public:
	static InputManager* GetInstance();
	static void Create();
	static void Destroy();

public:
	void SetInput(int keyCode);

	bool MouseLeftClicked() const;
	bool MouseMiddleClicked() const;
	bool MouseRightClicked() const;

	bool MouseLeftPressed() const;
	bool MouseMiddlePressed() const;
	bool MouseRightPressed() const;

	bool MouseLeftUp() const;
	bool MouseMiddleUp() const;
	bool MouseRightUp() const;

	void UpdateInput();

private:
	static InputManager* _pStatic_instance;

private:
	KeyInput _keyInput[256] = {};	// 255까지 사용하긴 함.
};