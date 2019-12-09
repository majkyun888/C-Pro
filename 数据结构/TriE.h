#pragma once
#include <map>
#include <string>
using namespace std;

class Trie {
private:
	struct Node {
		bool isWord;
		map<char, Node*> *next;
		Node(bool isWord) {
			this->isWord = isWord;
			next = new map<char, Node*>;
		}

		Node(): Node(false){}
	};
private:
	Node* root;
	int size;
public:
	Trie() {
		root = new Node;
		size = 0;
	}

	int getSize() { return size; }

	void add(string word) {

		Node* cur = root;

		for (char c : word) {
			auto ite = cur->next->find(c);
			if (ite == cur->next->end()) 
				cur->next->insert(make_pair(c, new Node));
			cur = cur->next->at(c);
		}

		if (!cur->isWord) {
			cur->isWord = true;
			size++;
		}
	}

	bool contains(string word) {
		Node* cur = root;
		for (char c : word) {
			auto ite = cur->next->find(c);
			if (ite == cur->next->end()) return false;

			cur = cur->next->at(c);
		}

		return cur->isWord;
	}

	bool isPrefix(string word) {
		Node* cur = root;
		for (char c : word) {
			auto ite = cur->next->find(c);
			if (ite == cur->next->end())
				return false;

			cur = cur->next->at(c);
		}

		return true;
	}
};