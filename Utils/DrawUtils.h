#pragma once
#pragma once
#include <Windows.h>
namespace Utils {
	inline HBRUSH CreateBrush(int r, int g, int b)
	{
		return CreateSolidBrush(RGB(r, g, b));
	}



}

void DrawBackGround(HDC hdc, PAINTSTRUCT ps);

void DrawNode(HDC hdc, PAINTSTRUCT ps);