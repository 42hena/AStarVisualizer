#pragma once

#include <Windows.h>

class AStar;
class Board;
class VisualizerEngine
{
public:
	VisualizerEngine();
	~VisualizerEngine();

	void Run();
	void Tick();
	void Render();

	void VisualWeight(HDC memDC);
	void VisualPath(HDC memDC);
	void VisualUIRender(HDC memDC);

	void SetHWND(HWND hWnd) { _hWnd = hWnd; }
public:
	static VisualizerEngine* GetInstance();
	static void Create();
	static void Destroy();


	static VisualizerEngine* _pStatic_instance;

private:
	/*AStar* pAStar = nullptr;
	Board* pBoard = nullptr;*/


	LARGE_INTEGER _lastTime;
	HWND _hWnd;
};