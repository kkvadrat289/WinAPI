#pragma once
#include <Windows.h>
#include <Windowsx.h>
#include <string>
#include "Game.h"

class CGameWindow
{
public:
	CGameWindow();
	~CGameWindow();

	static bool RegisterClass();
	bool Create(HWND parentHandle);
	void Show(int cmdShow);
	HWND GetHandle();
	void SetScoreHandle(HWND newScoreHandle);
protected:
	void OnPaint();
	void OnMouseClick(LPARAM lParam);
private:
	CGame game;
	HWND handle;
	HWND scoreHandle;
	RECT baseRect;
	HBRUSH brush;
	PAINTSTRUCT ps;
	bool scene;
	bool draw;
	int xPos;
	int yPos;
	int color;
	int territory;
	void printMessageInScore(std::string str);
	static LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

