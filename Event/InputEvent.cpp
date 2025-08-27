#include "InputEvent.h"
#include "../AStar/Vector2.h"

#include <tchar.h>
#include <Windows.h>

void MouseLeftButtonDownProcess(LPARAM lParam)
{
    wchar_t messageBuffer[256];

    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    Vector2 curPos(mousePosX, mousePosY);

    _stprintf_s(messageBuffer, _T("왼쪽 마우스 클릭 위치: (%d, %d)\n"), curPos._x, curPos._y);
    OutputDebugString(messageBuffer);
}

void MouseMiddleButtonDownProcess(LPARAM lParam)
{
    wchar_t messageBuffer[256];

    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    Vector2 curPos(mousePosX, mousePosY);

    _stprintf_s(messageBuffer, _T("가운데 마우스 클릭 위치: (%d, %d)\n"), curPos._x, curPos._y);
    OutputDebugString(messageBuffer);
}

void MouseRightButtonDownProcess(LPARAM lParam)
{
    wchar_t messageBuffer[256];

    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    Vector2 curPos(mousePosX, mousePosY);

    _stprintf_s(messageBuffer, _T("오른쪽 마우스 클릭 위치: (%d, %d)\n"), curPos._x, curPos._y);
    OutputDebugString(messageBuffer);
}

void MouseMovedProcess(LPARAM lParam)
{
    wchar_t messageBuffer[256];

    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    Vector2 curPos(mousePosX, mousePosY);

    _stprintf_s(messageBuffer, _T("마우스 움직임 위치: (%d, %d)\n"), curPos._x, curPos._y);
    OutputDebugString(messageBuffer);
}

void KeyboardProcess(WPARAM wParam)
{
    wchar_t messageBuffer[256];

    switch (wParam)
    {
    case VK_SPACE:
        {
            _stprintf_s(messageBuffer, _T("스페이스 바\n"));
            OutputDebugString(messageBuffer);
            break;
        }
    default:
        break;
    }
}
