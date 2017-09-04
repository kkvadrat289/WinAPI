#pragma once
#include <Windows.h>
#include <string>

class CFigure
{
public:
	CFigure();
	CFigure(int color);
	CFigure(int color, int xPos, int yPos);
	CFigure(int color, int cellNum);
	~CFigure();

	void DrawCircle(HDC hdc);
	void DrawSquare(HDC hdc);
	void DrawBorder(HDC hdc);
	void ChangeColor();
	void ChangeColor(int newColor);
	int GetColor();
	int findPosition(int xPos, int yPos);
	int calculateXPos(int numOfCell);
	int calculateYPos(int numOfCell);
	bool GetDirection(int i);
	void SetDirection(bool* dir);
	bool Border;
	bool ToRotate;
private:
	int row;
	int str;
	bool directions[9];
	int color;
	int cellNumber;
	HWND handle;
};

