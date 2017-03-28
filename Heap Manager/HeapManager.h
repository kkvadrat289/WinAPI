#pragma once

#include <Windows.h>
#include <cassert>
#include <map>
#include <iostream>

class CHeapManager
{
public:
	CHeapManager();
	~CHeapManager();
	void Create(int minSize, int maxSize_);
	void* Alloc(int size);
	void Free(void* mem);
	void Destroy();
	void printInfo();
private:
	//блоки памяти, каждый в своём хранилище соответственно размеру. 
	//bool: true, если блок свободен; int: размер блока в байтах
	std::map< void*, std::pair<bool, int> > blocksLess4;
	std::map< void*, std::pair<bool, int> > blocksMore4Less128;
	std::map< void*, std::pair<bool, int> > blocksMore128;
	//bool: true если страница закоммичена, int: количество блоков, пересекающихся с этой страницей
	std::map< void*, std::pair<bool, int> > pages;
	std::map< void*, std::pair<bool, int> >* blocks[3];
	static const int pageSize;
	int freeSize;
	int maxSize;
	void* reservedMemory;
	
	void printBlocks(std::map<void*, std::pair<bool, int>>* myMap);
	void printAllBlocks();
	void printPages();
	std::pair<void*,int> mergeBlocks(std::map<void*, std::pair<bool, int>>::iterator blockIt, int index);
	void* findFirstPage(void* address);
	int getIndexOfCurrentMap(int size);
	int getNumOfPages(void* pageAddress, void* address, int size);
	void insertBlock(int allocatedSize, int mapIndex, void* address);
	void setPagesCommited(void* address, int allocatedSize);
	void setPagesFree(void* address, int size);
	std::pair<int, void*> findPlace(int sizeToAlloc);
};

