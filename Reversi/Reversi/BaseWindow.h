#pragma once
#include <Windows.h>
#include "GameWindow.h"
#include "ScoreWindow.h"
#include "Game.h"


class CBaseWindow
{
public:
	CBaseWindow();
	~CBaseWindow();
	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);
	HWND GetHandle();
	void SetHandles();
protected:
	void OnPaint();
private:
	HWND handle;
	PAINTSTRUCT ps;
	CGameWindow gameWindow;
	CScoreWindow scoreWindow;
	int cellSize;
	int numOfCells;

	static LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

