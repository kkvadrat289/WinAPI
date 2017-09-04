#include <Windows.h>
#include <iostream>
#include "BaseWindow.h"
#include "GameWindow.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow) {

	CBaseWindow mainWindow;
	bool reg = mainWindow.RegisterClass();
	WNDCLASSEX wcx;
	if (!GetClassInfoEx(GetModuleHandle(NULL), L"MainWClass", &wcx)) {
		throw new std::logic_error("EllipseWindow class is not registered!\n");
	}
	mainWindow.Create();
	mainWindow.SetHandles();
	mainWindow.Show(nCmdShow);	 
	
	return 0;
}