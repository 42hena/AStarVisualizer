#include "DrawUtils.h"

//#include "../Global.h"
#include "../AStar/Board.h"
#include "../GDIManager.h"
#include "../Core/GlobalCore.h"
#include <Windows.h>
#include <tchar.h>

int boardSize = 50;

void DrawBackGround(HDC hdc, PAINTSTRUCT ps)
{
    {
        for (int i = 0; i < ps.rcPaint.right / boardSize + 1; ++i)
        {
            MoveToEx(hdc, i * boardSize, 0, NULL);  // 시작 좌표 지정
            LineTo(hdc, i * boardSize, ps.rcPaint.bottom);
        }

        for (int i = 0; i < ps.rcPaint.bottom / boardSize + 1; ++i)
        {
            MoveToEx(hdc, 0, i * boardSize, NULL);  // 시작 좌표 지정
            LineTo(hdc, ps.rcPaint.right, i * boardSize);
        }
    }

    SetBkMode(hdc, TRANSPARENT);
}

void DrawNode(HDC hdc, PAINTSTRUCT ps)
{
    HFONT hFont = CreateFont(
        12,                   // 글꼴의 높이 (원하는 크기로 조절)
        0,                    // 너비 (0으로 설정하면 시스템이 적절한 값 선택)
        0, 0,                 // 기울임각 및 회전각
        FW_NORMAL,            // 굵기
        FALSE, FALSE, FALSE,  // 이탤릭, 밑줄, 취소선
        DEFAULT_CHARSET,      // 문자 집합
        OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,    // 글꼴 품질 (글자를 부드럽게 보여줌)
        FIXED_PITCH | FF_MODERN, // ★★★★★ 이 부분이 중요합니다. ★★★★★
        L"Consolas"           // ★★★★★ 고정폭 폰트 이름 (Consolas, Courier New 등) ★★★★★
    );
    wchar_t buffer[100];


    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);


    int length = global_bufferLength;


    // 노드 생성
    for (int i = 0; i < 50; ++i)
    {
        for (int j = 0; j < 50; ++j)
        {
            int left = j * length + 1;
            int right = (j + 1) * length;
            int top = i * length + 1;
            int down = (i + 1) * length;
            RECT rect = { left, top, right, down };
            if (GDIManager::GetInstance() == nullptr)
                continue;
            if (global_board._board[i][j] == 0) {
                FillRect(hdc, &rect, GDIManager::GetInstance()->GetBrush("white"));
            }
            if (global_board._board[i][j] > 0)
            {
                if (global_board._board[i][j] == Start) {
                    FillRect(hdc, &rect, GDIManager::GetInstance()->GetBrush("red"));
                }
                else if (global_board._board[i][j] == Wall) {
                    FillRect(hdc, &rect, GDIManager::GetInstance()->GetBrush("black"));
                }
                else if (global_board._board[i][j] == End) {
                    FillRect(hdc, &rect, GDIManager::GetInstance()->GetBrush("blue"));
                }
                else if (global_board._board[i][j] == Open) {
                    FillRect(hdc, &rect, GDIManager::GetInstance()->GetBrush("green"));
                }
                else if (global_board._board[i][j] == Close) {
                    FillRect(hdc, &rect, GDIManager::GetInstance()->GetBrush("yellow"));
                }
                else if (global_board._board[i][j] == Path) {
                    FillRect(hdc, &rect, GDIManager::GetInstance()->GetBrush("turquoise"));
                }


                /*swprintf(buffer, 100, L"f: %.2lf", global_board._weight[i][j].f);
                TextOut(hdc, left, top + 10, buffer, static_cast<int>(wcslen(buffer)));*/

                rect.top = i * length;
                swprintf(buffer, 100, L"F: %2.2lf", global_board._weight[i][j].f);
                DrawText(hdc, buffer, -1, &rect, DT_LEFT | DT_SINGLELINE);

                rect.top += 10;
                swprintf(buffer, 100, L"G: %2.2lf", global_board._weight[i][j].g);
                DrawText(hdc, buffer, -1, &rect, DT_LEFT | DT_SINGLELINE);

                rect.top += 10;
                swprintf(buffer, 100, L"H: %2.2lf", global_board._weight[i][j].h);
                DrawText(hdc, buffer, -1, &rect, DT_LEFT | DT_SINGLELINE);
                //TextOut(hdc, left, top + 30, buffer, static_cast<int>(wcslen(buffer)));
            }
        }
    }

    // 1. F, G, W 출력.

    // 2. Close List까지의 현재 경로 출력.

    // 3. 
    

    

    SelectObject(hdc, hOldFont);


    DeleteObject(hFont);
}
