#include "ScoreWindow.h"



CScoreWindow::CScoreWindow()
{
}


CScoreWindow::~CScoreWindow()
{
}


bool CScoreWindow::RegisterClass() {
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
	wcx.lpszMenuName = L"Score";
	wcx.lpszClassName = L"CScoreWindow";
	wcx.hIconSm = NULL;

	return RegisterClassEx(&wcx);
}

bool CScoreWindow::Create(HWND parentHandle) {
	this->handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"CScoreWindow", L"Score", WS_CHILD | WS_BORDER | WS_OVERLAPPED | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, parentHandle, (HMENU)(1), GetModuleHandle(NULL), LPVOID(NULL));
	if (!this->handle) {
		wchar_t* error;
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&(error), 255, NULL);
		MessageBox(NULL, error, L"ScoreErr", MB_ICONERROR);
		return false;
	}
	whiteScore = 2;
	blackScore = 2;
	SetWindowLong(handle, GWL_USERDATA, (LONG)this);
	return handle;
}

void CScoreWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
}

LRESULT CALLBACK CScoreWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	CScoreWindow* window;

	switch (message) {
	case WM_PAINT:
		window = (CScoreWindow*)GetWindowLong(handle, GWL_USERDATA);
		window->OnPaint();
		break;
	case WM_USER:
		window = (CScoreWindow*)GetWindowLong(handle, GWL_USERDATA);
		window->OnWhite(lParam);
		break;
	case WM_USER + 1:
		window = (CScoreWindow*)GetWindowLong(handle, GWL_USERDATA);
		window->OnBlack(lParam);
		break;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}

HWND CScoreWindow::GetHandle() {
	return handle;
}

void CScoreWindow::SetGameHandle(HWND newGameHandle) {
	gameHandle = newGameHandle;
}															

void CScoreWindow::OnPaint() {
	BeginPaint(handle, &(ps));
	RECT baseRect;
	HBRUSH brush;
	RECT whiteTextRect;
	RECT blackTextRect;
	GetClientRect(handle, &(baseRect));
	brush = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
	SelectObject(ps.hdc, brush);
	SetDCBrushColor(ps.hdc, RGB(100, 100, 100));
	FillRect(ps.hdc, &(baseRect), brush);
	SetRect(&whiteRect, baseRect.left + 15, baseRect.top + 150, baseRect.right - 15, 200);
	SetRect(&whiteTextRect, whiteRect.left + 10, whiteRect.top + 10, whiteRect.right - 10, whiteRect.bottom - 10);
	SetDCBrushColor(ps.hdc, RGB(250, 250, 250));
	FillRect(ps.hdc, &whiteRect, brush);
	SetRect(&blackRect, baseRect.left + 15, baseRect.bottom - 250, baseRect.right - 15, baseRect.bottom - 200);
	SetRect(&blackTextRect, blackRect.left + 10, blackRect.top + 10, blackRect.right - 10, blackRect.bottom - 10);
	SetDCBrushColor(ps.hdc, RGB(60, 60, 60));
	FillRect(ps.hdc, &blackRect, brush);
	SetBkMode(ps.hdc, TRANSPARENT);
	DrawText(ps.hdc, L"White\nscore", 13, &whiteTextRect, DT_LEFT);
	DrawText(ps.hdc, L"Black\nscore",11, &blackTextRect, DT_LEFT);	
	wchar_t bufferWhite[256];
	wsprintfW(bufferWhite, L"%d", whiteScore);
	DrawText(ps.hdc, bufferWhite, 2, &whiteTextRect, DT_RIGHT);
	wchar_t bufferBlack[256];
	wsprintfW(bufferBlack, L"%d", blackScore);
	DrawText(ps.hdc, bufferBlack, 2, &blackTextRect, DT_RIGHT);
	EndPaint(handle, &(ps));
}

void CScoreWindow::OnWhite(LPARAM lParam) {
	whiteScore = *(int*)lParam;
	InvalidateRect(handle, (LPRECT)(&whiteRect), false);
}

void CScoreWindow::OnBlack(LPARAM lParam) {
	blackScore = *(int*)lParam;
	InvalidateRect(handle, (LPRECT)(&blackRect), false);
}