#include "BaseWindow.h"
#define CELL_SIZE 70
#define NUM_OF_CELLS 8
#define HEAD_SIZE 49
//Main window. Inclides Game and Score windows

CBaseWindow::CBaseWindow()
{
}


CBaseWindow::~CBaseWindow()
{
}

bool CBaseWindow::RegisterClass() {
	WNDCLASSEX wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = windowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = sizeof(void*);
	wcx.hInstance = GetModuleHandle(NULL);
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = NULL;
	wcx.lpszMenuName = L"MainMenu";
	wcx.lpszClassName = L"MainWClass";
	wcx.hIconSm = NULL;

	return RegisterClassEx(&wcx);
}

bool CBaseWindow::Create() {
	int windowSize = CELL_SIZE * NUM_OF_CELLS + 2 * (NUM_OF_CELLS - 1) + HEAD_SIZE;
	int scoreSize = 150; 
	this->handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWClass", L"Reversi", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		windowSize + scoreSize, windowSize, HWND(NULL), HMENU(NULL), GetModuleHandle(NULL), LPVOID(this));
	if (!this->handle) {
		wchar_t* error;
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&(error), 255, NULL);
		MessageBox(NULL, error, L"Invalid handle", MB_ICONERROR);
		return false;
	}
	this->gameWindow.RegisterClass();
	this->gameWindow.Create(this->handle);
	this->scoreWindow.RegisterClass();
	this->scoreWindow.Create(this->handle);
	SetWindowLong(handle, GWL_USERDATA, LONG(this));
	return handle;
}

void CBaseWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
	this->gameWindow.Show(cmdShow);
	this->scoreWindow.Show(cmdShow);

	bool message;
	MSG msg;

	while ((message = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (message == false)
		{
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam) {
	LPRECT parentRect;
	int i;
	i = GetWindowLong(hwndChild, GWL_ID);
	parentRect = (LPRECT)lParam;
	int x, y, weight, heigh;
	if (i == 0) {
		x = 0;
		y = 0;
		heigh = parentRect->bottom;
		weight = parentRect->bottom;
	}
	else {
		x = parentRect->bottom;
		y = 0;
		weight = parentRect->right - parentRect->bottom;
		heigh = parentRect->bottom;
	}
	MoveWindow(hwndChild, x, y, weight, heigh, TRUE);
	ShowWindow(hwndChild, SW_SHOW);
	return TRUE;
}

LRESULT CALLBACK CBaseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	CBaseWindow* data = (CBaseWindow*)lParam;
	RECT baseRect;
	switch (message) {
	case WM_SIZE:
		GetClientRect(handle, &baseRect);
		EnumChildWindows(handle, EnumChildProc, (LPARAM)&baseRect);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}

HWND CBaseWindow::GetHandle() {
	return this->handle;
}														

void CBaseWindow::SetHandles() {
	gameWindow.SetScoreHandle(scoreWindow.GetHandle());
	scoreWindow.SetGameHandle(gameWindow.GetHandle());
}