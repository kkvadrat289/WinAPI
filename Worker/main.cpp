#include "Trie.h"
#include <iostream>


int main() {
	Trie t;
	t.addPatterns(L"../words.txt");
	t.processText(L"../War.txt");
	
	
	return 0;
}