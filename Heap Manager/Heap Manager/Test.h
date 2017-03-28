#pragma once
#include <cmath>
#include <time.h>
#include "HeapManager.h"
#define ITER_NUM 100
CHeapManager MyHeap;
HANDLE DefHeap;

class CMyHeapTest
{
public:
	void* operator new(size_t size) {
		return MyHeap.Alloc(1024);
	}
	void operator delete(void* mem) {
		MyHeap.Free(mem);
	}
};


class CTest
{
public:
	void* operator new(size_t size) {
		return HeapAlloc(DefHeap, 0, 1024);
	}
	void operator delete(void*mem) {
		HeapFree(DefHeap, 0, mem);
	}

};

class CRandomTest
{
public:
	void* operator new(size_t size) {
		return MyHeap.Alloc(rand() % 1024 * 1024);
	}
	void operator delete(void* mem) {
		MyHeap.Free(mem);
	}
};


class CRandomDefaultTest
{
public:
	void* operator new(size_t size) {
		return HeapAlloc(DefHeap, 0, rand() % 1024 * 1024);
	}
	void operator delete(void* mem) {
		HeapFree(DefHeap, 0, mem);
	}
};



void MyHeapTest() {
	clock_t start, finish;
	MyHeap.Create(1024, 1024 * 1024);
	start = clock();
	CMyHeapTest* arr[ITER_NUM];
	for (int i = 0; i < ITER_NUM; ++i) {
		arr[i] = new CMyHeapTest;
	}
	for (int i = 0; i < ITER_NUM; ++i) {
		CMyHeapTest::operator delete(arr[i]);
	}
	MyHeap.Destroy();
	finish = clock();
	std::wcout << L"my heap time on simple test: " << float(finish - start)/CLOCKS_PER_SEC << std::endl;
}

void DefaultHeapTest() {
	clock_t start, finish;
	DefHeap = HeapCreate(0, 1024, 1024 * 1024);
	start = clock();
	CTest* arr[ITER_NUM];
	for (int i = 0; i < ITER_NUM; ++i) {
		arr[i] = new CTest;
	}
	for (int i = 0; i < ITER_NUM; ++i) {
		CTest::operator delete(arr[i]);
	}
	HeapDestroy(DefHeap);
	finish = clock();
	std::wcout << L"default heap time on simple test: " << float(finish - start) / CLOCKS_PER_SEC << std::endl;
}

void MyHeapRandomTest() {
	clock_t start, finish;
	MyHeap.Create(1024, 1024 * 1024 * ITER_NUM);
	start = clock();
	CRandomTest* arr[ITER_NUM];
	for (int i = 0; i < ITER_NUM; ++i) {
		arr[i] = new CRandomTest;
	}
	for (int i = 0; i < ITER_NUM; ++i) {
		CRandomTest::operator delete(arr[i]);
	}
	MyHeap.Destroy();
	finish = clock();
	std::wcout << L"my heap time on random test: " << float(finish - start) / CLOCKS_PER_SEC << std::endl;
}

void RandomDefaultHeapTest() {
	clock_t start, finish;
	DefHeap = HeapCreate(0, 1024, 1024 * 1024 * ITER_NUM);
	start = clock();
	CRandomDefaultTest* arr[ITER_NUM];
	for (int i = 0; i < ITER_NUM; ++i) {
		arr[i] = new CRandomDefaultTest;
	}
	for (int i = 0; i < ITER_NUM; ++i) {
		CRandomDefaultTest::operator delete(arr[i]);
	}
	HeapDestroy(DefHeap);
	finish = clock();
	std::wcout << L"default heap time on random test: " << float(finish - start) / CLOCKS_PER_SEC << std::endl;
}