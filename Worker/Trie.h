#pragma once
#include <vector>
#include <unordered_map>

struct Node {
	std::unordered_map<wchar_t, Node*> children;
	std::unordered_map<wchar_t, Node*> go;
	Node* parent;
	Node* suffixLinc;
	Node* up;
	wchar_t charToParent;
	bool isLeaf;
	std::vector<int> patternNumber;
};


class Trie {
public:
	Trie();
	~Trie();

	void processText(wchar_t* text);

private:
	int numberOfPatterns;
	Node* root;

	Node* getSuffixLinc(Node* vertex);
	Node* getLink(Node* vertex, wchar_t symbol);
	Node* getUp(Node* vertex);
	void addPattern(wchar_t* string);

};

