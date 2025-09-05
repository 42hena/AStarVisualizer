#include "VisualizerEngine.h"

#include "../GDIManager.h"
#include "../InputManager.h"
#include "../AStar/AStar.h"
#include "../AStar/Board.h"
#include "GlobalCore.h"

/*
*       static 변수 초기화
*/

VisualizerEngine* VisualizerEngine::_pStatic_instance = nullptr;

/*
*       static 맴버 함수
*/

VisualizerEngine* VisualizerEngine::GetInstance()
{
	return _pStatic_instance;
}

void VisualizerEngine::Create()
{
	_pStatic_instance = new VisualizerEngine();
}

void VisualizerEngine::Destroy()
{
	delete _pStatic_instance;
}

/*
*       VisualizerEngine의 특수 멤버 함수
*/

VisualizerEngine::VisualizerEngine()
{
    _hWnd = nullptr;

    // List로 구현된 AStar 생성
    pAStarList = new AStarWithList();

    // 우선순위 큐로 구현된 AStar 생성
    pAStarPQ = new AStarWithPQ();

    // 보드 생성
	pBoard = new Board();


	QueryPerformanceCounter(&_lastTime);
}

VisualizerEngine::~VisualizerEngine()
{
    // List로 만든 AStar 삭제
    if (pAStarList != nullptr)
    {
        delete pAStarList;
        pAStarList = nullptr;
    }

    // PQ로 만든 AStar 삭제
    if (pAStarPQ != nullptr)
    {
        delete pAStarPQ;
        pAStarPQ = nullptr;
    }

    // Board 삭제
    if (pBoard != nullptr)
    {
        delete pBoard;
        pBoard = nullptr;
    }
}

/*
*       
*/

void VisualizerEngine::Run()
{
	LARGE_INTEGER currentTime, freq;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&currentTime);

	float targetFrameRate = 60.0f;
	float oneFrameTime = 1.0f / targetFrameRate;
	float deltaTime = (float)(currentTime.QuadPart - _lastTime.QuadPart) / (float)freq.QuadPart;
	if (deltaTime >= oneFrameTime)
	{
        // Tick 함수
		Tick();

        // 그리기
		Render();

        // 
		_lastTime = currentTime;

        // Input 갱신
		InputManager::GetInstance()->UpdateInput();
	}
}

void VisualizerEngine::Tick()
{
    // Window 메시지가 여기로 와야 정상일 듯.
}

void VisualizerEngine::Render()
{
	HWND hWnd = _hWnd;
	HDC hdc = GetDC(hWnd);

	RECT rcClient;
	GetClientRect(hWnd, &rcClient);  // 클라이언트 영역 가져오기
	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	// 더블 버퍼링 권장
	HDC memDC = CreateCompatibleDC(hdc);
    if (memDC == nullptr)
    {
        DebugBreak();
        return;
    }
	HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

	// ---- 여기서 GDI로 그리기 ----
	
    // Render 전에 이전에 그려진 정보를 모두 지우는 함수
    ClearGridBackground(memDC);

    // Grid의 Boundary를 그리는 함수
    DrawGridLines(memDC);

    // 보드를 순회하며, 노드의 타입에 따라 그리는 함수
    DrawCellByState(memDC);

    // 상태에 따라 Debug 정보를 출력하는 함수
    DrawInfoByState(memDC);

    // UI 설명서를 출력하는 함수
    DrawUI(memDC);

	// ---- 그린걸 윈도우에 복사 ----
	BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

	// 정리
	SelectObject(memDC, oldBitmap);
	DeleteObject(memBitmap);
	DeleteDC(memDC);
	ReleaseDC(hWnd, hdc);
}

void VisualizerEngine::VisualNode(HDC memDC)
{
    Vector2 startPos = global_astar.GetStartPosition();
    Vector2 destPos = global_astar.GetDestPosition();

    int length = global_bufferLength;

    // 시작 지점 색칠하기
    {
        int left = startPos._x * length + 1;
        int right = (startPos._x + 1) * length;
        int top = startPos._y * length + 1;
        int down = (startPos._y+ 1) * length;
        RECT rect = { left, top, right, down };
        FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("red"));
    }

    // 목적 지점 색칠하기
    {
        int left = destPos._x * length + 1;
        int right = (destPos._x + 1) * length;
        int top = destPos._y * length + 1;
        int down = (destPos._y + 1) * length;
        RECT rect = { left, top, right, down };
        FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("blue"));
    }
}

void VisualizerEngine::VisualWeight(HDC memDC)
{
    wchar_t outputBuffer[256];
    int length = global_bufferLength;
    int maxBoardSize = global_bufferMaxCount;
    int fontSize = length / 4;

    // GDI 객체의 폰트 생성
    HFONT hUIFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");

    // 글씨 뒤의 배경을 투명하게 바꿈.
    SetBkMode(memDC, TRANSPARENT);

    SelectObject(memDC, hUIFont);
    for (int i = 0; i < maxBoardSize; ++i)
    {
        for (int j = 0; j < maxBoardSize; ++j)
        {
            int left = j * length + 1;
            int right = (j + 1) * length;
            int top = i * length + 1;
            int down = (i + 1) * length;
            RECT rect = { left, top, right, down };


            int type = global_board._board[i][j];
            // 현재 타입이 None이거나 장애물의 경우
            if (type == None || type == Wall) {
                continue;
            }

            rect.top = i * length + 1;
            swprintf(outputBuffer, 100, L"F:%2.2lf", global_board._weight[i][j].f);
            DrawText(memDC, outputBuffer, -1, &rect, DT_LEFT | DT_SINGLELINE);

            rect.top += fontSize;
            swprintf(outputBuffer, 100, L"G:%2.2lf", global_board._weight[i][j].g);
            DrawText(memDC, outputBuffer, -1, &rect, DT_LEFT | DT_SINGLELINE);

            rect.top += fontSize;
            swprintf(outputBuffer, 100, L"H:%2.2lf", global_board._weight[i][j].h);
            DrawText(memDC, outputBuffer, -1, &rect, DT_LEFT | DT_SINGLELINE);
        }
    }

    DeleteObject(hUIFont);
}

void VisualizerEngine::VisualPath(HDC memDC)
{
    wchar_t outputBuffer[256];
    int length = global_bufferLength;
    int maxBoardSize = global_bufferMaxCount;
    int fontSize = length / 4;

    Node* cur = global_astar.lastNode;
    while (cur)
    {
        Node* next = cur->parentNode;
        if (next != nullptr)
        {
            SelectObject(memDC, GDIManager::GetInstance()->GetBrush("red"));
            MoveToEx(memDC, cur->pos._x * global_bufferLength + 25, cur->pos._y * global_bufferLength + 25, NULL);  // 시작 좌표 지정
            LineTo(memDC, next->pos._x * global_bufferLength + 25, next->pos._y * global_bufferLength + 25);
            next = next->parentNode;
        }
        cur = cur->parentNode;
    }
}

void VisualizerEngine::DrawUI(HDC memDC)
{
    wchar_t outputBuffer[256];

    RECT rcClient;
    GetClientRect(_hWnd, &rcClient);  // 클라이언트 영역 가져오기
    RECT uiRect = { rcClient.right - 350, rcClient.top + 1, rcClient.right, 400 };

    FillRect(memDC, &uiRect, GDIManager::GetInstance()->GetBrush("white"));

    HFONT hUIFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");
    
    (HFONT)SelectObject(memDC, hUIFont);
    swprintf(outputBuffer, 100, L"A* 알고리즘 사용법");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"<마우스 이벤트>");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"왼쪽 클릭  : 시작 지점 설정");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"오른쪽 클릭 : 도착 지점 설정");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"가운데 클릭 후 드래그 : 장애물 설치/삭제");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 40;
    swprintf(outputBuffer, 100, L"<키보드 이벤트>");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);
    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"Space Bar : A* 1번 진행");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"A : 전부 진행");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"Q : H(x): 맨하튼");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"W : H(x): 유클리드");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"E : H(x): 옥타일");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);

    uiRect.top += 20;
    swprintf(outputBuffer, 100, L"R : H(x): 체비뭐시기");
    DrawText(memDC, outputBuffer, -1, &uiRect, DT_LEFT | DT_SINGLELINE);
}

void VisualizerEngine::ClearGridBackground(HDC memDC)
{
    HWND hWnd = _hWnd;
    HDC hdc = GetDC(hWnd);

    RECT rcClient;
    GetClientRect(hWnd, &rcClient);  // 클라이언트 영역 가져오기
    int width = rcClient.right - rcClient.left;
    int height = rcClient.bottom - rcClient.top;

    HBRUSH hBrush = GDIManager::GetInstance()->GetBrush("white");
    RECT rc = { 0, 0, width, height };
    FillRect(memDC, &rc, hBrush);
}

void VisualizerEngine::DrawGridLines(HDC memDC)
{
    HWND hWnd = _hWnd;
    HDC hdc = GetDC(hWnd);

    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    int width = rcClient.right - rcClient.left;
    int height = rcClient.bottom - rcClient.top;
    int length = global_bufferLength;

    // 세로 경계선 그리기
    for (int i = 0; i < Board::MAX_WIDTH; ++i)
    {
        MoveToEx(memDC, i * length, 0, NULL);
        LineTo(memDC, i * length, rcClient.bottom);
    }

    // 가로 경계선 그리기
    for (int i = 0; i < Board::MAX_HEIGHT; ++i)
    {
        MoveToEx(memDC, 0, i * length, NULL);
        LineTo(memDC, rcClient.right, i * length);
    }
}

void VisualizerEngine::DrawCellByState(HDC memDC)
{
    int length = global_bufferLength;

    // TODO: 50을 다른 걸로
    for (int i = 0; i < Board::MAX_HEIGHT; ++i)
    {
        for (int j = 0; j < Board::MAX_WIDTH; ++j)
        {
            int left = j * length + 1;
            int right = (j + 1) * length;
            int top = i * length + 1;
            int down = (i + 1) * length;
            RECT rect = { left, top, right, down };

            // GDIManager가 존재하지 않는 경우
            if (GDIManager::GetInstance() == nullptr) {
                continue;
            }

            // 노드 타입 확인 및 사각형 그리기
            int nodeType = global_board._board[i][j];
            switch (nodeType)
            {
            /*
            // 굳이 그릴 이유가 없어보임.
            case None:
                FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("white"));
                break;
            */
            case Start:
                FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("red"));
                break;
            case Wall:
                FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("black"));
                break;
            case End:
                FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("blue"));
                break;
            case Open:
                FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("green"));
                break;
            case Close:
                FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("yellow"));
                break;
            case Path:
                FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("turquoise"));
                break;
            default:
                break;
            }
        }
    }
}

void VisualizerEngine::DrawInfoByState(HDC memDC)
{
    switch (global_state)
    {
    case 0:
        VisualNode(memDC);
        break;
    case 1: // Weight
        VisualWeight(memDC);
        break;
    case 2: // Path
        VisualPath(memDC);
        break;
    default:
        break;
    }
}
