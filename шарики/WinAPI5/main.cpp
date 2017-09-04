#include <Windows.h>
#include <iostream>
#include "functions.h"
#include "CEllipseWindow.h"
#include "COverlappedWindow.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow) {

	COverlappedWindow mainWindow;
	bool reg = mainWindow.RegisterClass();
	WNDCLASSEX wcx;

	if (!GetClassInfoEx(GetModuleHandle(NULL), L"MainWClass", &wcx)) {
		throw new std::logic_error("EllipseWindow class is not registered!\n");
	}
	mainWindow.Create();
	mainWindow.Show(nCmdShow);	
	//SampleWindow(nCmdShow);
	return 0;
}
