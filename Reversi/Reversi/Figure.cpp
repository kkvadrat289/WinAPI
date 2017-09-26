#include "Figure.h"
  
CFigure::CFigure()
{
}

CFigure::CFigure(int _color) {
	this->color = _color;
	Border = false;
}

CFigure::CFigure(int _color, int xPos, int yPos) {
	color = _color;
	cellNumber = findPosition(xPos, yPos);
	Border = false;
}

CFigure::CFigure(int _color, int cellNum) {
	color = _color;
	cellNumber = cellNum;
	Border = false;
}

CFigure::~CFigure()
{
}

//Сама фишка. В клетке рисуется кружок
void CFigure::DrawCircle(HDC hdc) {
	COLORREF colorRGB;
	if (color == -1) {
		colorRGB = RGB(10, 10, 10);
		//colorRGB = RGB(46, 16, 0);
	}
	else if (color == 1) {
		colorRGB = RGB(250, 250, 250);
		//colorRGB = RGB(231, 210, 181);
	}
	else {
		return;
	}
	RECT circle;
	HBRUSH brush;
	brush = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
	SelectObject(hdc, brush);
	SetRect(&circle, calculateXPos(cellNumber) - 30, calculateYPos(cellNumber) - 30, calculateXPos(cellNumber) + 30, calculateYPos(cellNumber) + 30);
	SetDCBrushColor(hdc, colorRGB);
	Ellipse(hdc, circle.left, circle.top, circle.right, circle.bottom);
}

//закрашивание клетки - для визуализации возможных ходов
void CFigure::DrawSquare(HDC hdc) {
	COLORREF colorRGB;
	if (color == 0) {
		colorRGB = RGB(150, 150, 150);
		//colorRGB = RGB(243, 177, 142);
	}
	else if (color == -1){
		colorRGB = RGB(120, 120, 120);
		//colorRGB = RGB(118, 69, 39);
	}
	else {
		colorRGB = RGB(180, 180, 180);
		//colorRGB = RGB(205, 179, 135);
	}
	RECT square;
	HBRUSH brush;
	brush = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
	SelectObject(hdc, brush);
	SetRect(&square, calculateXPos(cellNumber) - 30, calculateYPos(cellNumber) - 30, calculateXPos(cellNumber) + 30, calculateYPos(cellNumber) + 30);
	SetDCBrushColor(hdc, colorRGB);
	FillRect(hdc, &square, brush);
}

//для отладки - закрашивает клетки, граничащие с уже стоящими фишками
void CFigure::DrawBorder(HDC hdc) {
	COLORREF colorRGB;
	if (Border == false) {
		//colorRGB = RGB(150, 150, 150);
		colorRGB = RGB(114, 117, 167);
	}
	else {
		colorRGB = RGB(100, 120, 120);		
	}
	RECT square;
	HBRUSH brush;
	brush = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
	SelectObject(hdc, brush);
	SetRect(&square, calculateXPos(cellNumber) - 35, calculateYPos(cellNumber) - 35, calculateXPos(cellNumber) + 35, calculateYPos(cellNumber) + 35);
	SetDCBrushColor(hdc, colorRGB);
	FillRect(hdc, &square, brush);
}

//-1:white, 1:black, 0:empty cell
void CFigure::ChangeColor() {
	color = -1 * color;
}

void CFigure::ChangeColor(int newColor) {
	color = newColor;
}

//по координатам курсора определяет, в какую клетку нужно рисовать
int CFigure::findPosition(int xPos, int yPos) {
	int hPosition, wPosition;
	for (int i = 0; i < 8; i++) {
		if (xPos >= i * 72 && xPos <= (i + 1) * 72) {
			hPosition = i;
			row = i;
		}
		if (yPos >= i * 72 && yPos <= (i + 1) * 72) {
			wPosition = i;
			str = i;
		}
	}
	int pos = 8 * wPosition + hPosition;
	return 8 * wPosition + hPosition;
}
							 
int CFigure::calculateXPos(int numOfCell) {
	return 35 + 72 * (numOfCell % 8);
}

int CFigure::calculateYPos(int numOfCell) {
	return 35 + 72 * (numOfCell / 8);
}

int CFigure::GetColor() {
	return color;
}

bool CFigure::GetDirection(int i) {
	if (directions[i])
		return true;
	else
		return false;
}

void CFigure::SetDirection(bool* dir) {
	for (int i = 0; i < 9; i++) {
		directions[i] = dir[i];
	}
}