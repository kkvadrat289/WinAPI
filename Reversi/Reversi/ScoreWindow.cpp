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
	SetWindowLong(handle, GWL_USERDATA, (LONG)this);
	return handle;
}

void CScoreWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
}

LRESULT CALLBACK CScoreWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	CScoreWindow* window = (CScoreWindow*)lParam;

	switch (message) {
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