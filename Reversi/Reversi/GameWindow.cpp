#include "GameWindow.h"



CGameWindow::CGameWindow()
{
}


CGameWindow::~CGameWindow()
{
}

bool CGameWindow::RegisterClass() {

	WNDCLASSEX wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_NOCLOSE;
	wcx.lpfnWndProc = windowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = sizeof(void*);
	wcx.hInstance = GetModuleHandle(NULL);
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = NULL;
	wcx.lpszMenuName = L"Game";
	wcx.lpszClassName = L"CGameWindow";
	wcx.hIconSm = NULL;

	return RegisterClassEx(&wcx);
}

bool CGameWindow::Create(HWND parentHandle) {
	this->handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"CGameWindow", L"Game", WS_CHILD | WS_BORDER | WS_OVERLAPPED | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, parentHandle, (HMENU)(0), GetModuleHandle(NULL), LPVOID(NULL));
	if (!this->handle) {
		wchar_t* error;
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&(error), 255, NULL);
		MessageBox(NULL, error, L"GameErr", MB_ICONERROR);
		return false;
	}
	SetWindowLong(handle, GWL_USERDATA, (LONG)this);
	game.InitGame();
	color = -1;
	RECT baseRect;
	GetClientRect(handle, &baseRect);
	InvalidateRect(handle, &baseRect, false);
	return handle;
}

void CGameWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
}

LRESULT CALLBACK CGameWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	CGameWindow* window = (CGameWindow*)lParam;
	CGameWindow* data;
	switch (message) {
	case WM_PAINT:
		data = (CGameWindow*)GetWindowLong(handle, GWL_USERDATA);										  
		data->OnPaint();
		break;
	case WM_LBUTTONDOWN:
		data = (CGameWindow*)GetWindowLong(handle, GWL_USERDATA);
		//data->draw = true;
		data->OnMouseClick(lParam);
		break;
	case WM_LBUTTONUP:
		data = (CGameWindow*)GetWindowLong(handle, GWL_USERDATA);
		//data->draw = false;
		break;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}

void CGameWindow::OnMouseClick(LPARAM lParam) {
	xPos = GET_X_LPARAM(lParam);
	yPos = GET_Y_LPARAM(lParam);
	bool res = game.ChangeState(xPos, yPos, color);
	if (res) {
		color = -1 * color;
	}
	else if (game.End) {
		if (game.blackScore > game.whiteScore)
			MessageBox(handle, L"Black wins!", L"THE END", MB_ICONERROR);
		else if (game.blackScore < game.whiteScore)
			MessageBox(handle, L"White wins!", L"THE END", MB_ICONERROR);
		else
			MessageBox(handle, L"Draw!", L"THE END", MB_ICONERROR);
	}
	game.UpdateAvailablePositions(color);

	InvalidateRect(handle, &(baseRect), false);
}

void CGameWindow::OnPaint() {
	BeginPaint(handle, &(ps));
	game.SetHDC(ps.hdc);
	GetClientRect(handle, &(baseRect));
	brush = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
	if (scene != true) {
		SelectObject(ps.hdc, brush);
		SetDCBrushColor(ps.hdc, RGB(150, 150, 150));
		FillRect(ps.hdc, &(baseRect), brush);
		int i = 0;
		RECT border;
		int cellSize = 70;
		for (; i < 7; i++) {
			SetRect(&border, 0, (i + 1) * cellSize + i * 2, baseRect.right, (i + 1)*cellSize + i * 2 + 2);
			SetDCBrushColor(ps.hdc, RGB(100, 100, 100));
			SelectObject(ps.hdc, brush);
			FillRect(ps.hdc, &border, brush);

			SetRect(&border, (i + 1) * cellSize + i * 2, 0, (i + 1) * cellSize + (i + 1) * 2, baseRect.bottom);
			SelectObject(ps.hdc, brush);
			FillRect(ps.hdc, &border, brush);
		}
		scene = true;
	}
	for (int i = 0; i < 64; i++) {
		if (game.availablePositions[i].GetColor() != -1 * color)
			game.availablePositions[i].DrawSquare(ps.hdc);
		game.gameState[i].DrawCircle(ps.hdc); 	
	}
	EndPaint(handle, &(ps));
	}


HWND CGameWindow::GetHandle() {
	return handle;
}

void CGameWindow::SetScoreHandle(HWND newScoreHandle) {
	this->scoreHandle = newScoreHandle;
}

