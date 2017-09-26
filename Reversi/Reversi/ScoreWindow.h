#pragma once
#include <Windows.h>
#include <string>

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
protected:
	void OnPaint();
	void OnWhite(LPARAM lparam);
	void OnBlack(LPARAM lParam);
private:
	HWND handle;
	HWND gameHandle;
	PAINTSTRUCT ps;
	RECT whiteRect;
	RECT blackRect;
	int blackScore;
	int whiteScore;
	static LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	
};

