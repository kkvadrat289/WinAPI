#pragma once
#include <vector>
#include <unordered_map>
#include <wchar.h>
#include <fstream>
#include <string>


struct Node {
	Node();

	std::unordered_map<wchar_t, Node*> children;
	std::unordered_map<wchar_t, Node*> go;
	Node* parent;
	Node* suffixLink;
	Node* up;
	wchar_t charToParent;
	bool isLeaf;
	size_t patternNumber;
};


class Trie {
public:
	Trie();
	~Trie();

	void processText(wchar_t* fileName);
	void addPatterns(wchar_t* fileName);
private:
	int numberOfPatterns;
	Node* root;
	std::vector<std::wstring> patterns;
	std::vector<std::pair<size_t, size_t> > positions;

	Node* getSuffixLink(Node* vertex);
	Node* getLink(Node* vertex, wchar_t symbol);
	Node* getUp(Node* vertex);
	void addPattern(std::wstring string);
	void clearText(wchar_t* fileName);

};

