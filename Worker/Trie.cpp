#include "Trie.h"
#define MAX_LENGTH 100
#include <iostream>
#define STR L"XXX"

Trie::Trie()
{
	root = new Node;

}


Trie::~Trie()
{
}

Node* Trie::getSuffixLink(Node* vertex)
{
	if( vertex == root || vertex->parent == root ) {
		vertex->suffixLink = root;
	} else {
		vertex->suffixLink = getLink(getSuffixLink(vertex->parent), vertex->charToParent);
	}
	return vertex->suffixLink;
}

Node* Trie::getLink(Node* vertex, wchar_t symbol)
{
	if( vertex->go.find(symbol) == vertex->go.end() ) {
		auto childIt = vertex->children.find(symbol);
		if( childIt != vertex->children.end() ) {
			vertex->go[symbol] = childIt->second;
		} else if( vertex == root ) {
			vertex->go[symbol] = root;
		} else {
			vertex->go[symbol] = getLink(getSuffixLink(vertex), symbol);
		}
	}
	return vertex->go[symbol];
}

Node* Trie::getUp(Node* vertex)
{
	if( !vertex->up ) {
		Node* suffLink = getSuffixLink(vertex);
		if( suffLink->isLeaf ) {
			vertex->up = suffLink;
		} else if ( suffLink == root ) {
			vertex->up = root;
		} else {
			vertex->up = getUp(getSuffixLink(vertex));
		}
	}
	return vertex->up;
}

void Trie::addPattern(std::wstring pattern)
{
	Node* current = root;
	auto symbolIt = pattern.begin();
	wchar_t symbol;
	int i = 0;
	while (symbolIt != pattern.end()) {
		symbol = *symbolIt;
		if ( current->children.find(symbol) == current->children.end() ) {
			current->children[symbol] = new Node();
			current->children[symbol]->parent = current;
			current->children[symbol]->charToParent = symbol;
		}
		current = current->children[symbol];
		i++;
		symbolIt++;
	} 
	current->isLeaf = true;
	current->patternNumber = numberOfPatterns + 1;
	numberOfPatterns++;
}

void Trie::addPatterns(wchar_t* fileName) {
	std::wifstream input(fileName);
	
	wchar_t symbol;
	std::wstring word;
	bool flag = true;
	while (input.get(symbol)) {
		while (!iswspace(symbol) && flag) {
			word.push_back(symbol);
			if (!input.get(symbol)) {
				flag = false;
			}
		}
		patterns.push_back(word);
		word.clear();
	}
	input.close();
	for (auto line : patterns) {
		this->addPattern(line);
	}
}

void Trie::processText(wchar_t* fileName) {
	std::wifstream input(fileName);
	Node* current = root;
	wchar_t symbol;
	size_t length = 0;
	size_t position = 0;
	while (input.get(symbol)) {
		current = getLink(current, symbol);
		if (current == root) {
			position += length;
			length = 1;
		} else if (current->isLeaf) {
			positions.push_back(std::make_pair(position, current->patternNumber));
			position += length;
			length = 1;
		} else {
			length++;
		}
	}
	for (auto item : positions) {
		std::wcout << item.first << L' ' << patterns[item.second - 1] << std::endl;
	}
	input.close();
}

void Trie::clearText(wchar_t* fileName) {
	std::wifstream is(fileName);

}

Node::Node() {
	children.clear();
	go.clear();
	suffixLink = NULL;
	parent = NULL;
	up = NULL;
	charToParent = L' ';
	isLeaf = false;
	patternNumber = -1;
}