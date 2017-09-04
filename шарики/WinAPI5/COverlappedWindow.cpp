#include "COverlappedWindow.h"
#include "resource.h"
#define NUM_OF_CHILDREN 4


COverlappedWindow::COverlappedWindow()
{
}


COverlappedWindow::~COverlappedWindow()
{
}

bool COverlappedWindow::RegisterClass() {
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

bool COverlappedWindow::Create() {
	this->handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWClass", L"Sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, HWND(NULL), HMENU(NULL), GetModuleHandle(NULL), LPVOID(this));
	if (!this->handle) {
		wchar_t* error;
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&(error), 255, NULL);
		MessageBox(NULL, error, L"Ellipsec", MB_ICONERROR);
		return false;
	}
	int i;
	for (i = 0; i < NUM_OF_CHILDREN; i++) {
		if (i == 0) {
			this->children[i].RegisterClass();
		}
		this->children[i].Create(handle, i);
	}
	this->accelerator = LoadAccelerators(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_ACCELERATOR1));
	SetWindowLong(handle, GWL_USERDATA, LONG(this));
	return handle;
}

void COverlappedWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
	int i;
	for (i = 0; i < NUM_OF_CHILDREN; i++) {
		this->children[i].Show(cmdShow);
	}
	bool message;
	MSG msg;
	while ((message = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (message == false)
		{
		}
		else if (!TranslateAccelerator(handle, accelerator, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}  
}


BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam) {
	LPRECT parentRect;
	int i;
	int idChild;
	i = GetWindowLong(hwndChild, GWL_ID);
	parentRect = (LPRECT)lParam;
	int x, y;
	if (i == 0 || i == 2) {
		x = 0;
	}
	else {
		x = 1;
	}
	if (i == 0 || i == 3) {
		y = 0;
	}
	else {
		y = 1;
	}
	MoveWindow(hwndChild, (parentRect->right / 2)*x, parentRect->bottom/2*y, parentRect->right / 2, parentRect->bottom/2, TRUE);
	ShowWindow(hwndChild, SW_SHOW);
	return TRUE;
}

LRESULT CALLBACK COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	COverlappedWindow* data = (COverlappedWindow*)lParam;
	int i;
	RECT baseRect;
	HWND h;
	int x, y, weight, heigh;
	switch (message) {	   
	case WM_CREATE:		
		break;
	case WM_SIZE:
		GetClientRect(handle, &baseRect);
		weight = baseRect.right / 2;
		heigh = baseRect.bottom / 2;
		EnumChildWindows(handle, EnumChildProc, (LPARAM)&baseRect);
		break;	
	case WM_COMMAND:
		data = (COverlappedWindow*)GetWindowLong(handle, GWL_USERDATA);
		switch (LOWORD(wParam)) {
		case ID_ACCELERATOR40001:
			h = data->children[3].GetHandle();
			SetFocus(h);
			break;
		case ID_ACCELERATOR40002:
			h = data->children[0].GetHandle();
			SetFocus(h);
			break;
		case ID_ACCELERATOR40003:
			h = data->children[2].GetHandle();
			SetFocus(h);
			break;
		case ID_ACCELERATOR40004:
			h = data->children[1].GetHandle();
			SetFocus(h);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}

HWND COverlappedWindow::GetHandle() {
	return this->handle;
}