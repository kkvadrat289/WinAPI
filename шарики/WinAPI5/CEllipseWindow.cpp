#include "CEllipseWindow.h"
#include "functions.h"
#include <random>


CEllipseWindow::CEllipseWindow()
{
}


CEllipseWindow::~CEllipseWindow()
{
}


bool CEllipseWindow::RegisterClass(){
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
	wcx.lpszClassName = L"ChildWClass";
	wcx.hIconSm = NULL;

	return RegisterClassEx(&wcx);
}

bool CEllipseWindow::Create(HWND parentHandle, int i) {
	this->handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"ChildWClass", L"Sample", WS_CHILD | WS_BORDER | WS_OVERLAPPED | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, parentHandle, (HMENU)(i), GetModuleHandle(NULL), LPVOID(NULL));
	if (!this->handle) {
		wchar_t* error;
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(),
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPWSTR)&(error), 255, NULL);
		MessageBox(NULL, error, L"Ellipsec", MB_ICONERROR);
		return false;
	}
	ellipse = new RECT;
	SetRect(ellipse, 10, 10, 100, 100);
	this->goRight = 1;
	this->goDown = 1;
	this->colorUp = 1;
	this->active = 0;
	color = 0;
	SetWindowLong(handle, GWL_USERDATA, (LONG)this);
	return handle;
}

void CEllipseWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
	/*bool message;
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
	}		  */
}


LRESULT CALLBACK CEllipseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	
	static COLORREF fillColor;
	static UINT_PTR timer;
	CEllipseWindow* data;
	timer = NULL;
	switch (message) {
	case WM_CREATE:
		SetTimer(handle, timer, 10, (TIMERPROC)NULL);
		break;
	case WM_PAINT:
		data = (CEllipseWindow*)GetWindowLong(handle, GWL_USERDATA);
		data->OnPaint(data);
		break;
	case WM_LBUTTONDOWN:
		data = (CEllipseWindow*)GetWindowLong(handle, GWL_USERDATA);
		if (GetFocus() != handle) {
			SetFocus(handle);
		}
		data->OnClick();
		break;
	case WM_TIMER:
		data = (CEllipseWindow*)GetWindowLong(handle, GWL_USERDATA);
		InvalidateRect(handle, data->ellipse, FALSE);
		break;

	case WM_DESTROY:
		data = (CEllipseWindow*)GetWindowLong(handle, GWL_USERDATA);
		data->OnDestroy(data);
		break;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}

void CEllipseWindow::OnDestroy(CEllipseWindow* data) {
	DeleteObject(data->brush);
	PostQuitMessage(0);
}

void CEllipseWindow::OnPaint(CEllipseWindow* data) {
	data->color = UpdateColor(data->color);
	GetClientRect(handle, &(data->baseRect));
	BeginPaint(handle, &(data->ps));
	if (data->compatibleDC) {
		DeleteObject(data->compatibleDC);
		DeleteObject(data->compatibleBitMap);
	} 
	data->compatibleDC = CreateCompatibleDC(data->ps.hdc);
	data->compatibleBitMap = CreateCompatibleBitmap(data->ps.hdc, data->baseRect.right - data->baseRect.left,data->baseRect.bottom - data->baseRect.top);

	SelectObject(data->compatibleDC, data->compatibleBitMap);

	data->brush = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
	SelectObject(data->compatibleDC, data->brush);
	SetDCBrushColor(data->compatibleDC, RGB(216, 136, 6));
	FillRect(data->compatibleDC, &(data->baseRect), data->brush);
	
	data->updateRightDown(data);
	SetRect(data->ellipse, data->ellipse->left + data->goRight*5*data->IsActive(), data->ellipse->top + data->goDown*3*data->IsActive(),
		data->ellipse->right + data->goRight*5*data->IsActive(), data->ellipse->bottom + data->goDown*3*data->IsActive());
	
	SetDCBrushColor(data->compatibleDC, RGB(color, color, 0));
	SelectObject(data->compatibleDC, data->brush);
	Ellipse(data->compatibleDC, data->ellipse->left + 10, data->ellipse->top + 10, data->ellipse->right - 10, data->ellipse->bottom - 10);

	BitBlt(data->ps.hdc, 0, 0, data->baseRect.right - data->baseRect.left, data->baseRect.bottom - data->baseRect.top, 
		data->compatibleDC, 0, 0, SRCCOPY);
	EndPaint(handle, &(data->ps));
}

HWND CEllipseWindow::GetHandle() {
	return this->handle;
}

int CEllipseWindow::UpdateColor(int color) {
	if (GetFocus() != handle) {
		return color;
	}
	if (color > 254) {
		colorUp = 0;
	}
	if (color < 1) {
		colorUp = 1;
	}
	if (colorUp) {
		color += 1;
	}
	else {
		color -= 1;
	}
	return color;

}

void CEllipseWindow::updateRightDown(CEllipseWindow* data) {
	if (data->ellipse->right >= data->baseRect.right && data->goRight) {
		data->goRight = -1;
	}
	else if (data->ellipse->left <= 1) {
		data->goRight = 1;
	}
	if (data->ellipse->bottom >= data->baseRect.bottom) {
		data->goDown = -1;
	}
	else if (data->ellipse->top <= 1) {
		data->goDown = 1;
	}
}


void CEllipseWindow::OnClick() {
	if (GetFocus() == handle) {
		active = 1;
	}
	else {
		active = 0;
	}
}

int CEllipseWindow::IsActive() {
	if (GetFocus() == handle) {
		return 1;
	}
	else {
		return 0;
	}
}

