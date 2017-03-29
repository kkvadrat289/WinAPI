#include "Trie.h"



Trie::Trie()
{
}


Trie::~Trie()
{
}

Node* Trie::getSuffixLinc(Node* vertex)
{
	if( vertex == root || vertex->parent == root ) {
		vertex->suffixLinc = root;
	} else {
		vertex->suffixLinc = getLink(getSuffixLinc(vertex->parent), vertex->charToParent);
	}
	return vertex->suffixLinc;
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
			vertex->go[symbol] = getLink(getSuffixLinc(vertex), symbol);
		}
	}
	return vertex->go[symbol];
}

Node* Trie::getUp(Node* vertex)
{
	if( !vertex->up ) {
		Node* suffLink = getSuffixLinc(vertex);
		if( suffLink->isLeaf ) {
			vertex->up = suffLink;
		} else if (suffLink == root) {
			vertex->up = root;
		} else {
			vertex->up = getUp(getSuffixLinc(vertex));
		}
	}
	return vertex->up;
}

void Trie::addPattern(wchar_t* pattern, int length)
{
	Node* current = root;
	for (int i = 0; i < length; i++) {

	}
}