#pragma once
#include "Figure.h"

class CGame
{
public:
	CGame();
	~CGame();
	void InitGame();
	bool ChangeState(int xPos, int yPos, int color);
	void UpdateAvailablePositions(int color);
	void SetHDC(HDC hdc);
	int findPosition(int xPos, int yPos);
	CFigure gameState[64];	
	CFigure availablePositions[64];
	bool End;
	int blackScore;
	int whiteScore;
	void ChangeScore(HWND handle);
private:
	bool directions[9];
	int offsets[9];
	int color;
	
	bool checkUp(int cellNum);
	bool checkUpRight(int cellNum);
	bool checkRight(int cellNum);
	bool checkDownRight(int cellNum);
	bool checkDown(int cellNum);
	bool checkDownLeft(int cellNum);
	bool checkLeft(int cellNum);
	bool checkUpLeft(int cellNum);
	void updateBorder(int cellNum);
	void rotate(int cellNum);
	int countAvailable();
	HDC hdc;
};

