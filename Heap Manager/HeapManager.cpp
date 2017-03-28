#include "HeapManager.h"

const int CHeapManager::pageSize = 4096;

CHeapManager::CHeapManager()
{
}


CHeapManager::~CHeapManager()
{
}

//�������� ���� ������� mxSize, ��������� ���������� ������ ������� minSize
void CHeapManager::Create(int minSize, int maxSize_)
{
	maxSize = maxSize_;
	if( minSize % pageSize != 0 ) {
		minSize = ( minSize / pageSize + 1 ) * pageSize;
	}
	if( maxSize % pageSize != 0 ) {
		maxSize = ( maxSize / pageSize + 1 ) * pageSize;
	}
	blocks[0] = &blocksLess4;
	blocks[1] = &blocksMore4Less128;
	blocks[2] = &blocksMore128;

	reservedMemory = VirtualAlloc(NULL, maxSize, MEM_RESERVE, PAGE_READWRITE);
	assert(reservedMemory);
	for( byte* address = (byte*)reservedMemory; address < (byte*)reservedMemory + maxSize; address += pageSize ) {
		pages[(void*)address].first = false;
		pages[(void*)address].second = 0;
	}

	setPagesCommited(reservedMemory, minSize);

	//���������� ���� ������: �������� minSize � ����������
	blocks[getIndexOfCurrentMap(minSize)]->insert(std::make_pair(reservedMemory, std::make_pair(false, minSize)));
	freeSize = maxSize - minSize;
	blocks[getIndexOfCurrentMap(freeSize)]->insert(std::make_pair((void*)( (byte*)reservedMemory + minSize ),
		std::make_pair(true, freeSize)));
}

//������������� ������ ������� size (� ������)
void* CHeapManager::Alloc(int size)
{
	int sizeToAlloc = size;
	if( sizeToAlloc % 4 != 0 )
		sizeToAlloc = ( sizeToAlloc / 4 + 1 ) * 4;
	if( freeSize < sizeToAlloc ) {
		std::wcout << L"Can't alloc. Wrong size\n";
		return nullptr;
	}
	//addresInfo = (����� ����, � ������� ��������� ����������� ����; ��� �����)
	std::pair<int, void*> addressInfo = findPlace(sizeToAlloc);
	int index = addressInfo.first;
	void* address = addressInfo.second;
	assert(address);
	insertBlock(sizeToAlloc, index, address);
	setPagesCommited(address, sizeToAlloc);
	return address;
}

//������������ ������ �� ��������� mem
void CHeapManager::Free(void* mem)
{
	std::map<void*, std::pair<bool, int>>::iterator blockIt;
	int index = -1;
	for( int i = 0; i < 3; ++i ) {
		blockIt = blocks[i]->find(mem);
		if( blockIt != blocks[i]->end() ) {
			index = i;
			break;
		}
	}
	setPagesFree(mem, blocks[index]->find(mem)->second.second);
	std::pair<void*, int> info = mergeBlocks(blockIt, index);
}

//���������� ����
void CHeapManager::Destroy()
{
	VirtualFree(pages.begin()->first, maxSize, MEM_RELEASE);
	int numOfPages = 0;

	//����� ������� ������
	for( auto page : pages ) {
		if( page.second.first ) {
			if( numOfPages == 0 ) {
				std::wcout << page.first << " => ";
			}
			++numOfPages;
		} else {
			if( numOfPages > 0 ) {
				std::wcout << numOfPages * pageSize << std::endl;
				numOfPages = 0;
			}
		}
	}
}

//������� �������� ��������� ������
std::pair<void*, int> CHeapManager::mergeBlocks(std::map<void*, std::pair<bool, int>>::iterator blockIt, int index)
{
	int size = blockIt->second.second;
	void* address = blockIt->first;
	void* newAddress = address;
	bool flag = false;

	//�������� ��������������� ����� � �������, ���� �� ��������
	blockIt--;
	if( blockIt != blocks[index]->end() && blockIt->second.first ) {
		newAddress = blockIt->first;
		size += blockIt->second.second;
		blocks[index]->erase(address);
		blocks[index]->insert(std::make_pair(newAddress, std::make_pair(true, size)));
		flag = true;
	}

	//�������� ���������� ����� � �������, ���� �� ��������
	blockIt = blocks[index]->find(newAddress);
	blockIt++;
	if( blockIt != blocks[index]->end() && blockIt->second.first ) {
		size += blockIt->second.second;
		blocks[index]->at(newAddress).second = size;
		address = blockIt->first;
		blocks[index]->erase(address);
		flag = true;
	}
	int newIndex = getIndexOfCurrentMap(size);
	//����������� � ������ ���, ���� ����� ������ �� �������� ��� ������ �������� ����
	if( index != newIndex && flag ) {
		blocks[index]->erase(newAddress);
		blocks[newIndex]->insert(std::make_pair(newAddress, std::make_pair(true, size)));
	}

	blocks[newIndex]->at(newAddress).first = true;
	return std::make_pair(newAddress, size);
}

//���������� �����, ������� ������� �����, ������� ������������ ����� ������������
std::pair<int, void*> CHeapManager::findPlace(int sizeToAlloc)
{
	int index = getIndexOfCurrentMap(sizeToAlloc);
	bool blockAddressFound = false;
	void* address;
	int indexOfMap = -1;
	for( int i = index; i < 3 && !blockAddressFound; ++i ) {
		for( auto it = blocks[i]->begin(); it != blocks[i]->end() && !blockAddressFound; ++it ) {
			if( it->second.first == true && it->second.second >= sizeToAlloc ) {
				blockAddressFound = true;
				address = it->first;
				indexOfMap = i;
			}
		}
	}
	assert(blockAddressFound);
	return std::make_pair(indexOfMap, address);
}

//���������� ������ ����, � ������� �������� ����
int CHeapManager::getIndexOfCurrentMap(int size)
{
	if( size <= pageSize )
		return 0;
	else if( size <= 1024 * 128 )
		return 1;
	else
		return 2;
}

void CHeapManager::insertBlock(int allocatedSize, int mapIndex, void* address)
{
	int sizeOfErasedBlock = blocks[mapIndex]->at(address).second;
	blocks[mapIndex]->erase(blocks[mapIndex]->find(address));
	blocks[getIndexOfCurrentMap(allocatedSize)]->insert(std::make_pair(address, std::make_pair(false, allocatedSize)));
	freeSize -= allocatedSize;
	if( freeSize ) {
		blocks[getIndexOfCurrentMap(freeSize)]->insert(std::make_pair((void*)( (byte*)address + allocatedSize ),
			std::make_pair(true, sizeOfErasedBlock - allocatedSize)));
	}
}

//���������� ��������� ������� (���������� ����������� �� �������� ������) � ������������, ���� �������� ��������
void CHeapManager::setPagesFree(void* address, int size)
{
	//����� ������ ��������, � ������� ������������ ���� � ���������� ����� �������
	void* firstPage = findFirstPage(address);
	assert(firstPage);
	int numPages = getNumOfPages(firstPage, address, size);

	//���������� ���������
	auto it = pages.find(firstPage);
	int sizeToFree = 0;
	void* pagesToFree = nullptr;
	bool startFree = false;
	for( int i = 0; i < numPages && it != pages.end(); ++i ) {
		if( it->second.second > 0 ) {
			it->second.second -= 1;
		}
		if( it->second.second == 0 && it->second.first == true ) {
			startFree = false;
			it->second.first = false;
			++sizeToFree;
			if( sizeToFree == 1 ) {
				pagesToFree = it->first;
			}
		} else {
			startFree = true;
		}
		if( startFree && sizeToFree ) {
			startFree = false;
			VirtualFree(pagesToFree, sizeToFree * pageSize, MEM_DECOMMIT);
			sizeToFree = 0;
		}
		++it;
	}
	if( sizeToFree ) {
		VirtualFree(pagesToFree, sizeToFree * pageSize, MEM_DECOMMIT);
	}
}

//��������� ��������, ���������� ���������, ������ �������
void CHeapManager::setPagesCommited(void* address, int size)
{
	void* firstPage = findFirstPage(address);
	assert(firstPage);
	const int numPages = getNumOfPages(firstPage, address, size);
	auto it = pages.find(firstPage);
	int sizeToCommit = 0;
	void* pagesToCommit = nullptr;
	bool startCommit = false;
	for( int i = 0; i < numPages && it != pages.end(); i++ ) {
		if( it->second.first ) {
			startCommit = true;
		} else {
			if( sizeToCommit == 0 ) {
				pagesToCommit = it->first;
			}
			sizeToCommit += pageSize;
			it->second.first = true;

		}
		if( startCommit && sizeToCommit ) {
			void* pointer = VirtualAlloc(pagesToCommit, sizeToCommit, MEM_COMMIT, PAGE_READWRITE);
			assert(pointer);
			startCommit = false;
			sizeToCommit = 0;
		}

		it->second.second += 1;
		++it;
	}
	if( sizeToCommit ) {
		void* pointer = VirtualAlloc(pagesToCommit, sizeToCommit, MEM_COMMIT, PAGE_READWRITE);
		assert(pointer);
		startCommit = false;
		sizeToCommit = 0;
	}
}

//���������� ������ ��������, � ������� ������������ ������ ����
void* CHeapManager::findFirstPage(void* address)
{
	std::map<void*, std::pair<bool, int>>::iterator it;
	it = pages.upper_bound(address);
	if( it != pages.end() ) {
		void* firstPage = (void*)( (byte*)( it->first ) - pageSize );
		return firstPage;
	}
	return nullptr;
}

//���������� ���������� �������, � �������� ������������ ������ ����
int CHeapManager::getNumOfPages(void* pageAdress, void* address, int size)
{
	int residual = (byte*)pageAdress + pageSize - (byte*)address;
	size -= residual;
	int num = size / pageSize;
	if( size - num * pageSize > 0 ) {
		++num;
	}
	++num;
	return num;
}



//------------------------------------------------------------------
//����� ����������

void CHeapManager::printInfo()
{
	printAllBlocks();
	printPages();
}

void CHeapManager::printBlocks(std::map<void*, std::pair<bool, int>>* myMap)
{
	std::map<void*, std::pair<bool, int>>::iterator it = myMap->begin();
	for( int i = 0; i < 10 && it != myMap->end(); ++it ) {
		std::wcout << it->first << L" => size: " << it->second.second << L"  free: " << it->second.first << std::endl;
		++i;
	}
}

void CHeapManager::printAllBlocks()
{
	if( blocksLess4.size() ) {
		std::wcout << L"Blocks less than 4 KB:\n";
		printBlocks(&blocksLess4);
	}
	if( blocksMore4Less128.size() ) {
		std::wcout << L"Blocks more than 4 KB less than 128 KB:\n";
		printBlocks(&blocksMore4Less128);
	}
	if( blocksMore128.size() ) {
		std::wcout << L"Blocks more than 128 KB:\n";
		printBlocks(&blocksMore128);
	}
}

void CHeapManager::printPages()
{
	std::wcout << L"Pages:\n";
	std::map<void*, std::pair<bool, int>>::iterator it = pages.begin();
	for( int i = 0; i < 10 && it != pages.end(); ++it ) {
		std::wcout << it->first << " => " << it->second.first << L' ' << it->second.second << std::endl;
		++i;
	}
}