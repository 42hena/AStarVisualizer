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
	float deltaTime = (float)(currentTime.QuadPart - _lastTime.QuadPart)
			/ (float)freq.QuadPart;
	if (deltaTime >= oneFrameTime)
	{
		Tick();
		Render(); 
		wchar_t messageBuffer[256];
		/*swprintf(messageBuffer, 256, L"deltaTime: %f\n", deltaTime);
		OutputDebugString(messageBuffer);*/
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

	for (int i = 0; i < width / 50 + 1; ++i)
	{
		MoveToEx(memDC, i * 50, 0, NULL);  // 시작 좌표 지정
		LineTo(memDC, i * 50, rcClient.bottom);
	}

	for (int i = 0; i < width / 50 + 1; ++i)
	{
		MoveToEx(memDC, 0, i * 50, NULL);  // 시작 좌표 지정
		LineTo(memDC, rcClient.right, i * 50);
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
            int left = j * 50 + 1;
            int right = (j + 1) * 50;
            int top = i * 50 + 1;
            int down = (i + 1) * 50;
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

                rect.top = i * 50;
                swprintf(buffer, 100, L"F: %2.2lf", global_board._weight[i][j].f);
                DrawText(memDC, buffer, -1, &rect, DT_LEFT | DT_SINGLELINE);

                rect.top += 10;
                swprintf(buffer, 100, L"G: %2.2lf", global_board._weight[i][j].g);
                DrawText(memDC, buffer, -1, &rect, DT_LEFT | DT_SINGLELINE);

                rect.top += 10;
                swprintf(buffer, 100, L"H: %2.2lf", global_board._weight[i][j].h);
                DrawText(memDC, buffer, -1, &rect, DT_LEFT | DT_SINGLELINE);
                //TextOut(hdc, left, top + 30, buffer, static_cast<int>(wcslen(buffer)));
            }
        }
    }
	// ---- 그린걸 윈도우에 복사 ----
	BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, hOldFont);


    DeleteObject(hFont);







	// 정리
	SelectObject(memDC, oldBitmap);
	DeleteObject(memBitmap);
	DeleteDC(memDC);
	ReleaseDC(hWnd, hdc);
}
