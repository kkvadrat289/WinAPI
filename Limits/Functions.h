#pragma once

#include <iostream>
#include <Windows.h>

void AllocateMemory()
{
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	std::wcout << L"Hardware information: \n" << L"  OEM ID: " << siSysInfo.dwOemId << L"\n  Number of processors: "
		<< siSysInfo.dwNumberOfProcessors << L"\n  Page size: " << siSysInfo.dwPageSize << L"\n  Processor type: "
		<< siSysInfo.dwProcessorType << L"\n  Minimum application address: " << siSysInfo.lpMinimumApplicationAddress
		<< L"\n  Maximum application address: " << siSysInfo.lpMaximumApplicationAddress << L"\n  Active processor mask: "
		<< siSysInfo.dwActiveProcessorMask << L"\n  Allocation Granularity: " << siSysInfo.dwAllocationGranularity <<
		std::endl;

	SIZE_T additionalSize = siSysInfo.dwAllocationGranularity;
	SIZE_T allocatedSize = additionalSize * 16;
	SIZE_T answer = 0;
	DWORD messageID;
	void* ptr = nullptr;
	do {
		ptr = VirtualAlloc(NULL, allocatedSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (ptr) {
			answer += allocatedSize;
		}
	} while (ptr);
	do {
		ptr = VirtualAlloc(NULL, additionalSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (ptr) {
			answer += additionalSize;
		}
	} while (ptr);

	messageID = GetLastError();
	std::wcout << L"\nLast error: " << messageID << L"\nMax Size can be reserved: " << answer / 1024 / 1024 << "MB" << std::endl;
	system("pause");
}

void AllocWithFragmentation(){
	void* ptr = nullptr;
	SIZE_T sizeToAlloc;
	SIZE_T allocatedMemory = 0;
	int flag = rand() % 2;
	do {
		sizeToAlloc = (SIZE_T)(rand() % 1000000);
		if (flag) {
			ptr = VirtualAlloc(NULL, sizeToAlloc, MEM_COMMIT | MEM_RESERVE, PAGE_GUARD | PAGE_READWRITE);
		} else {
			ptr = VirtualAlloc(NULL, sizeToAlloc, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		}

		if (ptr) {
			allocatedMemory += sizeToAlloc;
		}
	} while (ptr);
	DWORD messageID = GetLastError();
	std::wcout << L"Last error: " << messageID << L"\n Allocated memory: " << allocatedMemory/1024/1024 << "MB" << std::endl;
	system("pause");
}

void KernelObjects() {
	HANDLE eventHandle = CreateEvent(NULL, TRUE, TRUE, NULL);
	HANDLE dupEventHandle;
	bool res;
	long int handlesCount = 0;
	do {
		res = DuplicateHandle(GetCurrentProcess(), eventHandle, GetCurrentProcess(), &dupEventHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
		++handlesCount;
		if (handlesCount % 100000 == 0) {
			std::wcout << handlesCount << std::endl;
		}
	} while (res);
	DWORD messageID = GetLastError();
	std::wcout << L"Last error: " << messageID << L"\nNumber of handles: " << handlesCount << std::endl;
	if (messageID == 1450) {
		std::wcout << L"ERROR_NO_SYSTEM_RESOURCES" << std::endl;
	}
	system("pause");
}

void CreatingPens()
{
	HPEN penHandle;
	long int penCount = -1;
	do {
		penHandle = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		++penCount;
	} while (penHandle);
	std::wcout << L"Number of created pens: " << penCount << std::endl;
	system("pause");
}

void EmptyFunction() {
	int q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h;
	EmptyFunction();
}

