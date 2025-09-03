#include "VisualizerEngine.h"

#include "../GDIManager.h"
#include "../InputManager.h"
#include "../AStar/AStar.h"
#include "../AStar/Board.h"
#include "GlobalCore.h"

VisualizerEngine* VisualizerEngine::_pStatic_instance = nullptr;

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

VisualizerEngine::VisualizerEngine()
{
    _hWnd = nullptr;
	/*pAStar = new AStar();
	pBoard = new Board();*/
	QueryPerformanceCounter(&_lastTime);
}

VisualizerEngine::~VisualizerEngine()
{
}

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
		Tick();
		Render();
		_lastTime = currentTime;

		InputManager::GetInstance()->UpdateInput();
	}
}

void VisualizerEngine::Tick()
{
	wchar_t messageBuffer[256];
	// 왼쪽 버튼 이벤트 추가
	if (InputManager::GetInstance()->MouseLeftClicked() == true)
	{
		swprintf(messageBuffer, 256, L"마우스 왼쪽 버튼 Clickf\n");
		OutputDebugString(messageBuffer);
	}
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
    }
	HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

	// ---- 여기서 GDI로 그리기 ----
	//Rectangle(memDC, 10, 10, 100, 100);
	HBRUSH hBrush = GDIManager::GetInstance()->GetBrush("white");
	RECT rc = { 0, 0, width, height };
	FillRect(memDC, &rc, hBrush);

    int length = global_bufferLength;

	for (int i = 0; i < 50; ++i)
	{
		MoveToEx(memDC, i * length, 0, NULL);  // 시작 좌표 지정
		LineTo(memDC, i * length, rcClient.bottom);
	}

	for (int i = 0; i < 50; ++i)
	{
		MoveToEx(memDC, 0, i * length, NULL);  // 시작 좌표 지정
		LineTo(memDC, rcClient.right, i * length);
	}

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


    HFONT hOldFont = (HFONT)SelectObject(memDC, hFont);

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
                FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("white"));
            }
            if (global_board._board[i][j] > 0)
            {
                if (global_board._board[i][j] == Start) {
                    FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("red"));
                }
                else if (global_board._board[i][j] == Wall) {
                    FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("black"));
                }
                else if (global_board._board[i][j] == End) {
                    FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("blue"));
                }
                else if (global_board._board[i][j] == Open) {
                    FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("green"));
              
                }
                else if (global_board._board[i][j] == Close) {
                    FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("yellow"));
                    
                }
                else if (global_board._board[i][j] == Path) {
                    FillRect(memDC, &rect, GDIManager::GetInstance()->GetBrush("turquoise"));
                }


                /*swprintf(buffer, 100, L"f: %.2lf", global_board._weight[i][j].f);
                TextOut(hdc, left, top + 10, buffer, static_cast<int>(wcslen(buffer)));*/

                
                //TextOut(hdc, left, top + 30, buffer, static_cast<int>(wcslen(buffer)));
            }
        }
    }

    



    switch (global_state)
    {
    case 0:
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


    VisualUIRender(memDC);

	// ---- 그린걸 윈도우에 복사 ----
	BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, hOldFont);


    DeleteObject(hFont);




    /*
    * UI
    */
    


	// 정리
	SelectObject(memDC, oldBitmap);
	DeleteObject(memBitmap);
	DeleteDC(memDC);
	ReleaseDC(hWnd, hdc);
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

    for (int i = 0; i < maxBoardSize; ++i)
    {
        for (int j = 0; j < maxBoardSize; ++j)
        {
            int left = j * length + 1;
            int right = (j + 1) * length;
            int top = i * length + 1;
            int down = (i + 1) * length;
            RECT rect = { left, top, right, down };
            /*if (GDIManager::GetInstance() == nullptr)
                continue;*/

            // 1

            int type = global_board._board[i][j];
            if (type != Close) {
                continue;
            }

            Node* cur = global_board.p[i][j];
            Node* next = cur->parentNode;
            while (cur)
            {
                if (next != nullptr)
                {
                    SelectObject(memDC, GDIManager::GetInstance()->GetBrush("red"));
                    MoveToEx(memDC, cur->_nodePos._x * global_bufferLength + 25, cur->_nodePos._y * global_bufferLength + 25, NULL);  // 시작 좌표 지정
                    LineTo(memDC, next->_nodePos._x * global_bufferLength + 25, next->_nodePos._y * global_bufferLength + 25);
                    next = next->parentNode;
                }
                cur = cur->parentNode;
            }
        }
    }
}

void VisualizerEngine::VisualUIRender(HDC memDC)
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
