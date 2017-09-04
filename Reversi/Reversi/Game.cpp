#include "Game.h"



CGame::CGame()
{
}


CGame::~CGame()
{
}

void CGame::InitGame() {
	offsets[1] = -8;
	offsets[2] = -7;
	offsets[3] = 1;
	offsets[4] = 9;
	offsets[5] = 8;
	offsets[6] = 7;
	offsets[7] = -1;
	offsets[8] = -9;
	for (int i = 0; i < 64; i++) {
		gameState[i] = CFigure(0, i);
		availablePositions[i] = CFigure(0, i);
	}
	gameState[27].ChangeColor(1);
	gameState[36].ChangeColor(1);
	gameState[28].ChangeColor(-1);
	gameState[35].ChangeColor(-1);
	for (int i = 0; i < 64; i++) {
		if (gameState[i].GetColor() == 0)
			updateBorder(i);
	}
	color = -1;
	blackScore = 2;
	whiteScore = 2;
	UpdateAvailablePositions(-1);
}

void CGame::SetHDC(HDC _hdc) {
	hdc = _hdc;
}

bool CGame::ChangeState(int xPos, int yPos, int color) {
	this->color = color;
	int cellNum = findPosition(xPos, yPos);
	if (countAvailable() == 0)
		return false;
	if (countAvailable() == -1)
		End = true;
	if (availablePositions[cellNum].GetColor() == 0) 
		return false;
	gameState[cellNum].ChangeColor(color);
	gameState[cellNum].findPosition(xPos, yPos);
	gameState[cellNum].Border = false;
	availablePositions[cellNum].ChangeColor(0);
	for (int i = 0; i < 64; i++) {
		if (gameState[i].GetColor() == 0)
			updateBorder(i);
	}
	if (color == 1)
		whiteScore++;
	else
		blackScore++;
	rotate(cellNum);
	return true;
}

int CGame::findPosition(int xPos, int yPos) {
	int hPosition, wPosition;
	for (int i = 0; i < 8; i++) {
		if (xPos >= i * 72 && xPos <= (i + 1) * 72) {
			hPosition = i;
		}
		if (yPos >= i * 72 && yPos <= (i + 1) * 72) {
			wPosition = i;
		}
	}
	int pos = 8 * wPosition + hPosition;
	return 8 * wPosition + hPosition;
}

void CGame::UpdateAvailablePositions(int color) {
	int current;
	this->color = color;
	bool available;
	for (int i = 0; i < 64; i++) {
		available = false;
		for (int j = 0; j < 9; j++)
			directions[j] = false;
		if (gameState[i].Border) {
			if (checkUp(i))
				available = true;
			if (checkUpRight(i))
				available = true; 
			if (checkRight(i))
				available = true; 
			if (checkDownRight(i))
				available = true;
			if (checkDown(i))
				available = true; 
			if (checkDownLeft(i))
				available = true; 
			if (checkLeft(i)) 
				available = true; 
			if (checkUpLeft(i))
				available = true;			
		}
		if (available) {
			availablePositions[i].ChangeColor(color);
			availablePositions[i].SetDirection(directions);
		}
		else {
			availablePositions[i].ChangeColor(0);
		}
	}
}

void CGame::updateBorder(int cellNum) {
	if (
		(gameState[cellNum].GetColor() == 0) &&
		//левый край
		((cellNum % 8 != 0 && gameState[cellNum - 1].GetColor() != 0) ||
		//правый край
		((cellNum + 1) % 8 != 0 && gameState[cellNum + 1].GetColor() != 0) ||
	    //верхний край
		(cellNum >= 8 && gameState[cellNum - 8].GetColor() != 0) ||
		//нижний край
		(cellNum <= 55 && gameState[cellNum + 8].GetColor() != 0) ||
		//влево вниз
		(cellNum % 8 != 0 && cellNum <= 55 && gameState[cellNum + 7].GetColor() != 0) ||
		//вправо вниз
		((cellNum + 1) % 8 != 0 && cellNum <= 55 && gameState[cellNum + 9].GetColor() != 0) ||
		//влево вверх
		(cellNum % 8 != 0 && cellNum >= 8 && gameState[cellNum - 9].GetColor() != 0) ||
		//вправо вверх
		((cellNum + 1) % 8 != 0 && cellNum >= 8 && gameState[cellNum - 7].GetColor() != 0)
	)	) {
		gameState[cellNum].Border = true;
	}
	else if (gameState[cellNum].GetColor() != 0)
	{
		gameState[cellNum].Border = false;
	}	
}

bool CGame::checkUp(int cellNum) {
	int current = cellNum - 8;
	while (current >= 0 && gameState[current].GetColor() == -1 * color) {
		current -= 8;
	}
	if (current >= 0 && gameState[current].GetColor() == color && current != cellNum - 8) {
		directions[1] = true;
		return true;
	}
	return false;
}
bool CGame::checkUpRight(int cellNum) {
	int current = cellNum - 7;
	while (current >= 0 && current % 8 != 0 && gameState[current].GetColor() == -1 * color) {
		current -= 7;
	}
	if (current >= 0 && current % 8 != 0 && gameState[current].GetColor() == color && current != cellNum - 7) {
		directions[2] = true;
		return true;
	}
	return false;
}
bool CGame::checkRight(int cellNum) {
	int current = cellNum + 1;
	while (current % 8 != 0 && gameState[current].GetColor() == -1 * color) {
		current += 1;
	}
	if (current % 8 != 0 && gameState[current].GetColor() == color && current != cellNum + 1) {
		directions[3] = true;
		return true;
	}
	return false;
}
bool CGame::checkDownRight(int cellNum) {
	int current = cellNum + 9;
	while (current < 64 && current % 8 != 0 && gameState[current].GetColor() == -1 * color) {
		current += 9;
	}
	if (current < 64 && current % 8 != 0 && gameState[current].GetColor() == color && current != cellNum + 9) {
		directions[4] = true;
		return true;
	}
	return false;
}
bool CGame::checkDown(int cellNum) {
	int current = cellNum + 8;
	while (current < 64 && gameState[current].GetColor() == -1 * color) {
		current += 8;
	}
	if (current < 64 && gameState[current].GetColor() == color && current != cellNum + 8) {
		directions[5] = true;
		return true;
	}
	return false;
}
bool CGame::checkDownLeft(int cellNum) {
	int current = cellNum + 7;
	while (current < 64 && (current + 1) % 8 != 0 && gameState[current].GetColor() == -1 * color) {
		current += 7;
	}
	if (current < 64 && (current + 1) % 8 != 0 && gameState[current].GetColor() == color && current != cellNum + 7) {
		directions[6] = true;
		return true;
	}
	return false;
}
bool CGame::checkLeft(int cellNum) {
	int current = cellNum - 1;
	while ((current + 1) % 8 != 0 && gameState[current].GetColor() == -1 * color) {
		current -= 1;
	}
	if ((current + 1) % 8 != 0 && gameState[current].GetColor() == color && current != cellNum - 1) {
		directions[7] = true;
		return true;
	}
	return false;
}
bool CGame::checkUpLeft(int cellNum) {
	int current = cellNum - 9;
	while (current >= 0 && (current + 1) % 8 != 0 && gameState[current].GetColor() == -1 * color) {
		current -= 9;
	}
	if (current >= 0 && (current + 1) % 8 != 0 && gameState[current].GetColor() == color && current != cellNum - 9) {
		directions[8] = true;
		return true;
	}
	return false;
}

void CGame::rotate(int cellNum) {
	for (int i = 1; i < 9; i++) {
		if (availablePositions[cellNum].GetDirection(i)) {
			int current = cellNum;
			do {
				current = current + offsets[i];
				gameState[current].ChangeColor(color);
				gameState[current].ToRotate = true;
				if (color == 1) {
					whiteScore++;
					blackScore--;
				}
				else {
					whiteScore--;
					blackScore++;
				}
			} while (gameState[current + offsets[i]].GetColor() != color);	
		}
	}
}

int CGame::countAvailable() {
	int count = 0;
	for (int i = 0; i < 64; i++)
		if (availablePositions[i].GetColor() == color)
			count++;
	if (count == 0) {
		for (int i = 0; i < 64; i++)
			if (availablePositions[i].GetColor() == -1 * color)
				count++;
		if (count == 0)
			return -1;
	}
	return count;
}