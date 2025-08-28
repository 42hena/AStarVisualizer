#ifndef __INPUT_EVENT_H__
#define __INPUT_EVENT_H__

#include <Windows.h>

void MouseLeftButtonDownProcess(LPARAM lParam);
void MouseRightButtonDownProcess(LPARAM lParam);
void MouseMiddleButtonDownProcess(LPARAM lParam);
void MouseMiddleButtonUpProcess(LPARAM lParam);
void MouseMovedProcess(LPARAM lParam);

void KeyboardProcess(WPARAM wParam);

void PaintProcess(HDC hdc, PAINTSTRUCT ps);

#endif
