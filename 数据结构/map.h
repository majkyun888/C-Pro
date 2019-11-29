#pragma once
#include <iostream>
using namespace std;


template <typename K, typename V> 
struct MapInterface
{
	virtual void add(K key, V value) = 0;
	virtual V remove(K key) = 0;
	virtual bool contains(K key) = 0;
	virtual V get(K key) = 0;
	virtual void set(K key, V value) = 0;
	virtual int getSize() = 0;
	virtual bool isEmpty() = 0;
};


template <typename K, typename V>
class Map: public MapInterface<K, V>
{
private:
	struct Node {
		K key;
		V value;
		Node* next;
		Node(K key, V value, Node* next)
		{
			this->key = key;
			this->value = value;
			this->next = next;
		}
		Node(K key, V value): Node(key, value, NULL){}

		Node(K key): Node(key, 0) {}
		
		Node()
		{
			this->key = NULL;
			this->value = 0;
			this->next = NULL;
		}
	};
private:
	Node* dummyHead;
	int size;
	Node* getNode(K key)
	{
		Node* cur = dummyHead->next;
		while (NULL != cur)
		{
			if (cur->key == key)
				return cur;
			cur = cur->next;
		}
		return NULL;
	}

	Node* Destoty(Node* node)
	{
		if (NULL == node)
			return NULL;

		node->next = Destoty(node->next);
		delete node;
		node = NULL;
		return node;
	}
public:
	Map(): dummyHead(new Node), size(0) {}
	~Map() {
		if (dummyHead)
		{
			Destoty(dummyHead);
		}
	}

	int getSize() { return size; }

	bool isEmpty() { return size == 0; }

	bool contains(K key) { return getNode(key) != NULL; }

	V get(K key) {
		Node* temp = getNode(key);
		return temp == NULL ? 0 : temp->value;
	}

	void add(K key, V value){
		Node* node = getNode(key);

		if (NULL == node) {
			dummyHead->next = new Node(key, value, dummyHead->next);
			size++;
		}
		else
		{
			node->value = value;
		}
	}

	void set(K key, V newValue)
	{
		Node* node = getNode(key);
		if (NULL == node) {
			throw - 99;
		}
		else
		{
			node->value = newValue;
		}
	}

	V remove(K key)
	{
		Node* cur = dummyHead;
		while (NULL != cur)
		{
			if (cur->next->key == key)
				break;
			cur = cur->next;
		}

		if (NULL != cur->next)
		{
			Node* delNode = cur->next;
			cur->next = delNode->next;
			V temp = delNode->value;
			size--;
			delete delNode;
			delNode = NULL;
			return temp;
		}
		
		return NULL;
	}
};


template <typename K, typename V>
class BSTMap : public MapInterface<K, V>
{
private:
	struct Node
	{
		K key;
		V value;
		Node* left, * right;
		Node(K key, V value)
		{
			this->key = key;
			this->value = value;
			left = NULL;
			right = NULL;
		}
	};
private:
	Node* root;
	int size;

	Node* add(Node* node, K key, V value)
	{
		if (NULL == node)
		{
			size++;
			return new Node(key, value);
		}
		if (key < node->key)
			node->left = add(node->left, key, value);
		else if (key > node->key)
			node->right = add(node->right, key, value);
		else
			node->value = value;

		return node;
	}

	Node* getNode(Node* node, K key)
	{

		if (NULL == node)
			return NULL;

		if (key < node->key) {
			return getNode(node->left, key);
		}
		else if (key > node->key) {
			return getNode(node->right, key);
		}
		else
			return node;
	}



	Node* minimum(Node* node)
	{
		if (NULL == node->left)
			return node;

		return minimum(node->left);
	}

	Node* maximum(Node* node)
	{
		if (NULL == node->right)
			return node;

		return maximum(node->right);
	}

	Node* removeMin(Node* node)
	{
		if (NULL == node->left)		//3-left
		{
			Node* nodeRight = node->right;
			node->right = NULL;
			delete node;
			size--;
			return nodeRight;
		}

		node->left = removeMin(node->left);		//3->left
		return node;
	}

	Node* removeMax(Node* node)
	{
		if (NULL == node->right)
		{
			Node* nodeLeft = node->left;
			node->left = NULL;
			delete node;
			size--;
			return nodeLeft;
		}

		node->right = removeMax(node->right);
		return node;
	}

	Node* remove(Node* node, K key)
	{
		if (NULL == node)
			return NULL;

		if (key < node->key)
		{
			node->left = remove(node->left, key);		//5->left = remove(3)
			return node;
		}
		else if (key > node->key)
		{
			node->right = remove(node->right, key);
			return node;
		}
		else  //3
		{
			if (NULL == node->left)
			{
				Node* nodeRight = node->right;
				node->right = NULL;
				delete node;
				size--;
				return nodeRight;
			}

			if (NULL == node->right)
			{
				Node* nodeLeft = node->left;
				node->left = NULL;
				delete node;
				size--;
				return nodeLeft;
			}

			Node* successor = new Node(minimum(node->right)->key, minimum(node->right)->value);		//3->right
			successor->left = node->left;
			successor->right = removeMin(node->right);
			//successor->right = node->right;
			node->left = node->right = NULL;
			delete node;
			return successor;
		}
	}

	void Destory(Node* node)
	{
		if (NULL == node) return;

		Destory(node->left);
		Destory(node->right);
		node->left = node->right = NULL;
		delete node;
		node = NULL;
	}
public:
	BSTMap()
	{
		root = NULL;
		size = 0;
	}

	~BSTMap()
	{
		if (root)
		{
			Destory(root);
		}
		root = NULL;
	}

	bool isEmpty() { return size == 0; }

	bool contains(K key)
	{
		return getNode(root, key) != NULL;
	}

	void add(K key, V value) { root = add(root, key, value); }	//add element

	int getSize() { return size; }		//tree size

	bool isEmpty() const { return size == 0; }

	void set(K key, V value)
	{
		Node* node = getNode(root, key);
		if (NULL == node)
			throw - 99;

		node->value = value;
	}

	V get(K key)
	{
		Node* node = getNode(root, key);

		return node == NULL ? 0 : node->value;
	}


	V remove(K key)
	{
		Node* node = getNode(root, key);
		if (node != NULL)
		{
			root = remove(root, key);
			return node->value;
		}

		return NULL;
	}
};

