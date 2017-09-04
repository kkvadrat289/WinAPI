#pragma once
#include <Windows.h>
#include "CEllipseWindow.h"


class COverlappedWindow
{
public:
	COverlappedWindow();
	~COverlappedWindow();
	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);
	HWND GetHandle();
	void ResizeChildWindows();
private:
	HWND handle;
	HACCEL accelerator;
	CEllipseWindow children[4];
	static LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

