#include "InputEvent.h"
#include "../AStar/Vector2.h"
#include "../AStar/Board.h"
#include "../Utils/DrawUtils.h"
#include "../Core/GlobalCore.h"
#include "../AStar/AStar.h"
#include "../InputManager.h"

#include <tchar.h>
#include <Windows.h>

void MouseLeftButtonDownProcess(LPARAM lParam)
{
    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    int length = global_bufferLength;

    int boardXIndex = mousePosX / length;
    int boardYIndex = mousePosY / length;
    
    // 시작 지점이 이미 존재하는 지 확인.
    if (global_astar.HasStartPos() == true) {
        // 이전 시작 지점을 None 상태로 변경
        global_board._board[global_astar.GetStartPosY()][global_astar.GetStartPosX()] = 0;
    }
    
    // 시작점 세팅 플래그를 True로 변경
    global_astar.SetStartTrueFlag();
    
    // 시작 목표 지점 좌표 세팅
    Vector2 newPosition(boardXIndex, boardYIndex);
    global_astar.SetStartPosition(newPosition);

    // 시작 목표 지점 좌표 세팅
    if (global_board._board[boardYIndex][boardXIndex] == Close) {
        global_astar.SetDestFalseFlag();
    }

    // 현재 Board 좌표를 Start 상태로 변경
    global_board._board[boardYIndex][boardXIndex] = Start;
}

void MouseMiddleButtonDownProcess(LPARAM lParam)
{
    // 마우스 가운데 버튼 Pressed 상태를 나타내기 위한 플레그 설정
    global_mouseMiddleButton = true;
}

void MouseMiddleButtonUpProcess(LPARAM lParam)
{
    // 마우스 가운데 버튼을 때면 false로 변경.
    global_mouseMiddleButton = false;
}

void MouseRightButtonDownProcess(LPARAM lParam)
{
    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    int length = global_bufferLength;

    int boardXIndex = mousePosX / length;
    int boardYIndex = mousePosY / length;

    // 도착 지점이 이미 존재하는 지 확인.
    if (global_astar.HasDestPos() == true) {
        // 이전 도착 지점을 None 상태로 변경
        global_board._board[global_astar.GetDestPosY()][global_astar.GetDestPosX()] = None;
    }

    // 도착점 세팅 플래그를 True로 변경
    global_astar.SetDestTrueFlag();

    // 도착 목표 지점 좌표 세팅
    Vector2 newPosition(boardXIndex, boardYIndex);
    global_astar.SetDestPosition(newPosition);

    // 현재 지점이 시작 좌표의 경우 시작 지점의 플레그를 삭제.
    if (global_board._board[boardYIndex][boardXIndex] == Start) {
        global_astar.SetStartFalseFlag();
    }

    // 현재 Board 좌표를 End 상태로 변경
    global_board._board[boardYIndex][boardXIndex] = End;
}

void MouseMovedProcess(LPARAM lParam)
{
    int mousePosX = LOWORD(lParam);
    int mousePosY = HIWORD(lParam);

    int length = global_bufferLength;

    // 마우스 중간 버튼이 눌린 상태의 경우
    if (global_mouseMiddleButton)
    {
        int boardXIndex = mousePosX / length;
        int boardYIndex = mousePosY / length;

        // 현재 지점이 시작 좌표의 경우 시작 지점의 플레그를 삭제.
        if (global_board._board[boardYIndex][boardXIndex] == Start) {
            global_astar.SetStartFalseFlag();
        }

        // 현재 지점이 도착 좌표의 경우 도착 지점의 플레그를 삭제.
        if (global_board._board[boardYIndex][boardXIndex] == End) {
            global_astar.SetDestFalseFlag();
        }

        // 현재 좌표를 Wall 상태로 변경
        global_board._board[boardYIndex][boardXIndex] = Wall;
    }
}

void TryEnlargeSize()
{
    // 최대 사이즈보다 커지는거 방지
    if (global_bufferLength * 2 >= 192) {    // TODO: 매직 넘버로 바꿔야 함.
        return;
    }
    global_bufferLength = global_bufferLength * 2;
}

void TryShrinkSize()
{
    // 최소 사이즈보다 작아지는거 방지
    if (global_bufferLength / 2 <= 12) {    // TODO: 매직 넘버로 바꿔야 함.
        return;
    }
    global_bufferLength = global_bufferLength / 2;
}

void ChangeDebugState()
{
    global_state = (global_state + 1) % 3;
}

void SearchAll()
{
    // ClearLevel 상태 이전까지 돌기
    while (global_astar._state != ClearLevel) {
        global_astar.SearchWithList();
    }
}

void KeyboardProcess(WPARAM wParam)
{
    switch (wParam)
    {
    case VK_ESCAPE:
        {
            PostQuitMessage(0);
        }
        break;
    case VK_SPACE:
        {
            global_astar.SearchWithList();
        }
        break;
    case 'A':
        {
            SearchAll();
        }
        break;
    case 'Z':
        {
            ChangeDebugState();
        }
    break;
    case VK_PRIOR:  // PageUp
        {
            TryEnlargeSize();
        }
        break;
    case VK_NEXT:   // PageDown
        {
            TryShrinkSize();
        }
        break;
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
