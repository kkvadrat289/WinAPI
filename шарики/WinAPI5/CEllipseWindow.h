#pragma once
#include <Windows.h>

class CEllipseWindow
{
public:
	CEllipseWindow();
	~CEllipseWindow();

	static bool RegisterClass();
	bool Create(HWND parentHandle, int i);
	void Show(int cmdShow);
	HWND GetHandle();
	int IsActive();
protected:
	void OnDestroy(CEllipseWindow* data);
	void OnPaint(CEllipseWindow* data);
	void OnClick();
private:
	HWND handle;
	int color;
	HDC compatibleDC;
	HBITMAP compatibleBitMap;
	HBRUSH brush;
	RECT baseRect;
	PAINTSTRUCT ps;
	UINT_PTR timer;
	RECT *ellipse;
	int goRight;
	int goDown;
	int colorUp;
	int active;

	static LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	int UpdateColor(int color);
	void updateRightDown(CEllipseWindow* data);
};

