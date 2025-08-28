#include "InputEvent.h"
#include "../AStar/Vector2.h"
#include "../AStar/Board.h"
#include "../Utils/DrawUtils.h"
#include "../Core/GlobalCore.h"
#include "../AStar/AStar.h"

#include <tchar.h>
#include <Windows.h>

int boardSize1 = 50; // TODO

void MouseLeftButtonDownProcess(LPARAM lParam)
{
    wchar_t messageBuffer[256];

    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    int boardXIndex = mousePosX / boardSize1;
    int boardYIndex = mousePosY / boardSize1;
    
    // 시작점이 있는 지 확인
    if (global_astar.HasStartPos() == true) {
        // None으로 밀기
        global_board._board[global_astar.GetStartPosY()][global_astar.GetStartPosX()] = 0;
    }
    
    // 플래그 변경
    global_astar.SetStartTrueFlag();
    
    // astar의 시작점 좌표 세팅
    Vector2 newPosition(boardXIndex, boardYIndex);
    global_astar.SetStartPosition(newPosition);

    // board 바꾸기.
    if (global_board._board[boardYIndex][boardXIndex] == 3) {
        global_astar.SetDestFalseFlag();
    }
    global_board._board[boardYIndex][boardXIndex] = 1;
}

void MouseMiddleButtonDownProcess(LPARAM lParam)
{
    wchar_t messageBuffer[256];

    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    Vector2 curPos(mousePosX, mousePosY);

    // 가운데 버튼 클릭
    global_mouseMiddleButton = true;

    _stprintf_s(messageBuffer, _T("가운데 마우스 클릭 위치: (%d, %d)\n"), curPos._x, curPos._y);
    OutputDebugString(messageBuffer);
}

void MouseMiddleButtonUpProcess(LPARAM lParam)
{
    global_mouseMiddleButton = false;
}

void MouseRightButtonDownProcess(LPARAM lParam)
{
    wchar_t messageBuffer[256];

    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);
    int boardXIndex = mousePosX / boardSize1;
    int boardYIndex = mousePosY / boardSize1;
    // 시작점이 있는 지 확인
    if (global_astar.HasStartPos() == true) {
        // None으로 밀기
        global_board._board[global_astar.GetDestPosY()][global_astar.GetDestPosX()] = 0;
    }

    // 플래그 변경
    global_astar.SetDestTrueFlag();

    // astar의 시작점 좌표 세팅
    Vector2 newPosition(boardXIndex, boardYIndex);
    global_astar.SetDestPosition(newPosition);

    if (global_board._board[boardYIndex][boardXIndex] == 1) {
        global_astar.SetStartFalseFlag();
    }

    // board 바꾸기.
    global_board._board[boardYIndex][boardXIndex] = 3;
}

void MouseMovedProcess(LPARAM lParam)
{
    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    // 벽 색칠하기
    if (global_mouseMiddleButton)
    {
        int boardPoxX = mousePosX / 50;
        int boardPoxY = mousePosY / 50;

        if (global_board._board[boardPoxY][boardPoxX] == 1) {
            global_astar.SetStartFalseFlag();
        }
        if (global_board._board[boardPoxY][boardPoxX] == 3) {
            global_astar.SetDestFalseFlag();
        }
        global_board._board[boardPoxY][boardPoxX] = Wall; // TODO
    }
}

void KeyboardProcess(WPARAM wParam)
{
    wchar_t messageBuffer[256];

    switch (wParam)
    {
    case VK_SPACE:
        {
            global_astar.SearchWithList();
            _stprintf_s(messageBuffer, _T("스페이스 바\n"));
            OutputDebugString(messageBuffer);
            break;
        }
    default:
        break;
    }
}

void PaintProcess(HDC hdc, PAINTSTRUCT ps)
{
    // 1. 배경화면 그리기
    DrawBackGround(hdc, ps);

    // 2. 노드정보 그리기
    DrawNode(hdc, ps);
}
