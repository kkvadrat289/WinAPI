#include "functions.h"


void SampleWindow(int nCmdShow) {
	WNDCLASSEX wcx;

	wcx.cbSize = sizeof(wcx);          // size of structure 
	wcx.style = CS_HREDRAW | CS_VREDRAW;                    // redraw if size changes 
	wcx.lpfnWndProc = MainWndProc;     // points to window procedure 
	wcx.cbClsExtra = 0;                // no extra class memory 
	wcx.cbWndExtra = 0;                // no extra window memory 
	wcx.hInstance = GetModuleHandle(NULL);         // handle to instance 
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);              // predefined app. icon 
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);                    // predefined arrow 
	wcx.hbrBackground = NULL;                  // white background brush 
	wcx.lpszMenuName = L"MainMenu";    // name of menu resource 
	wcx.lpszClassName = L"MainWClass";  // name of window class 
	wcx.hIconSm = NULL;

	RegisterClassEx(&wcx);

	HWND hwnd;
	hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWClass", L"Sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, HWND(NULL), HMENU(NULL), GetModuleHandle(NULL), LPVOID(NULL));

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	bool bRet;
	MSG msg;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit	
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}