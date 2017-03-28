// Автор: Деткова Ю.Н., 493 группа
//

#include "Functions.h"

int wmain()
{
	//AllocateMemory();
	//AllocWithFragmentation();
	//KernelObjects();
	//CreatingPens();
	//EmptyFunction();
	std::wcout << L"Max path length: " << MAX_PATH << std::endl;
	wchar_t nameBuffer[MAX_PATH];
	wchar_t sysNameBuffer[MAX_PATH];
	DWORD maxComponentLength;
	DWORD fileFlags;
	DWORD VSNumber;
	GetVolumeInformation(NULL, nameBuffer, sizeof(nameBuffer), &VSNumber, &maxComponentLength, &fileFlags, sysNameBuffer, sizeof(sysNameBuffer));
	std::wcout << L"max component length: " << maxComponentLength << std::endl;

	return 0;
}