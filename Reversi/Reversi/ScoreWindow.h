#pragma once
#include <Windows.h>

class CScoreWindow
{
public:
	CScoreWindow();
	~CScoreWindow();
	static bool RegisterClass();
	bool Create(HWND parentHandle);
	void Show(int cmdShow);
	HWND GetHandle();
	void SetGameHandle(HWND newGameHandle);
private:
	HWND handle;
	HWND gameHandle;
	static LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

