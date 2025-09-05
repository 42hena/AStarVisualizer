#pragma once

#include <Windows.h>

class AStarWithList;
class AStarWithPQ;
class Board;
class VisualizerEngine
{
public:
	VisualizerEngine();
	~VisualizerEngine();

public:
	void Run();

	void SetHWND(HWND hWnd) { _hWnd = hWnd; }

private:	/* 이벤트 함수 */
	void Tick();
	void Render();

private:	/* 그리기 함수들 */
	void ClearGridBackground(HDC memDC);
	void DrawGridLines(HDC memDC);
	void DrawCellByState(HDC memDC);
	void DrawInfoByState(HDC memDC);
	void DrawUI(HDC memDC);

	void VisualNode(HDC memDC);
	void VisualWeight(HDC memDC);
	void VisualPath(HDC memDC);

public:
	static VisualizerEngine* GetInstance();
	static void Create();
	static void Destroy();

private:
	static VisualizerEngine* _pStatic_instance;

private:
	AStarWithList*	pAStarList	= nullptr;
	AStarWithPQ*	pAStarPQ	= nullptr;
	Board*			pBoard		= nullptr;

	LARGE_INTEGER _lastTime;
	HWND _hWnd;
};