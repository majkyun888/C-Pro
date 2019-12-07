#pragma once
using namespace std;

#pragma once
#include <iostream>
#include <stack>
#include <queue>
#include <list>
using namespace std;

template <typename K, typename V>
class AVLTree
{
private:
	struct Node
	{
		K key;
		V value;
		int height;
		Node* left, * right;
		Node(K key, V value)
		{
			this->key = key;
			this->value = value;
			left = NULL;
			right = NULL;
			height = 1;
		}
	};
private:
	Node* root;

	int size;

	Node* rightRotate(Node* y) {
		Node* x = y->left;
		Node* T3 = x->right;
		x->right = y;

		y->left = T3;

		y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
		x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

		return x;
	}

	Node* leftRotate(Node* y) {
		Node* x = y->right;
		Node* T2 = x->left;

		x->left = y;
		y->right = T2;

		y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
		x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

		return x;
	}

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

		//¸üÐÂheight
		node->height = 1 + max(getHeight(node->left), getHeight(node->right));

		//¼ÆËãbalanceFactor
		int balanceFactor = getBalanceFactor(node);

		/*if (abs(balanceFactor) > 1)
			cout << "This is not balance" << endl;*/

		//LL
		if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
		{
			cout << "is balance" << isBalanced() << endl;
			return rightRotate(node);
		}
		
		//LR
		if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
			cout << "is need LR " << isBalanced() << endl;
			node->left = leftRotate(node->left);
			return rightRotate(node);
		}

		//RR
		if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
		{
			cout << "left Rotate" << isBalanced() << endl;
			return leftRotate(node);
		}

		//RL
		if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
			cout << "is need RL " << isBalanced() << endl;
			node->right = rightRotate(node->right);
			return leftRotate(node);
		}

		return node;
	}

	void Print(Node* node)
	{
		if (NULL == node)
			return;

		cout << node->e << endl;
		Print(node->left);
		Print(node->right);
	}

	void inOrder(Node* node, list<K>& keys)
	{
		if (NULL == node)
			return;

		inOrder(node->left, keys);
		//cout << node->e << endl;
		keys.push_back(node->key);
		inOrder(node->right, keys);
	}

	void postOrder(Node* node)
	{
		if (NULL == node)
			return;

		postOrder(node->left);
		postOrder(node->right);
		cout << node->e << endl;
	}

	void nonPreOrder(Node* node)
	{
		Node* cur = node;
		stack<Node*> nd;
		nd.push(cur);

		while (!nd.empty())
		{
			Node* temp = nd.top();
			nd.pop();
			cout << temp->e << endl;

			if (NULL != temp->right)
				nd.push(temp->right);

			if (NULL != temp->left)
				nd.push(temp->left);
		}
	}

	void levelOrder(Node* node)
	{
		Node* cur = node;
		queue<Node*> que;
		que.push(node);

		while (!que.empty())
		{
			Node* temp = que.front();
			que.pop();
			cout << temp->e << endl;

			if (NULL != temp->left) que.push(temp->left);

			if (NULL != temp->right) que.push(temp->right);
		}
	}

	bool contains(Node* node,K key)
	{
		if (NULL == node)
			return false;

		if (key < node->key)
			return contains(node->left, key);
		else if (key > node->key)
			return contains(node->right, key);
		else if (key == node->key)
			return true;
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

		Node* retNode = NULL;

		if (key < node->key)
		{
			node->left = remove(node->left, key);		//5->left = remove(3)
			retNode = node;
		}
		else if (key > node->key)
		{
			node->right = remove(node->right, key);
			retNode = node;
		}
		else  //3
		{
			if (NULL == node->left)
			{
				Node* nodeRight = node->right;
				node->right = NULL;
				delete node;
				size--;
				retNode = nodeRight;
			}
			else if (NULL == node->right)
			{
				Node* nodeLeft = node->left;
				node->left = NULL;
				delete node;
				size--;
				retNode = nodeLeft;
			}
			else
			{
				Node* successor = new Node(minimum(node->right)->key, minimum(node->right)->value);		//3->right
				successor->left = node->left;
				successor->right = removeMin(node->right);
				//successor->right = node->right;
				node->left = node->right = NULL;
				delete node;
				retNode = successor;
			}
			
		}

		if (NULL == retNode) return NULL;

		retNode->height = 1 + max(getHeight(retNode->left), getHeight(retNode->right));

		int balanceFactor = getBalanceFactor(retNode);

		//LL
		if (balanceFactor > 1 && getBalanceFactor(retNode->left) >= 0)
			return rightRotate(retNode);

		//LR
		if (balanceFactor > 1 && getBalanceFactor(retNode->left) < 0)
		{
			retNode->left = leftRotate(retNode->left);
			return rightRotate(retNode);
		}

		//RR
		if (balanceFactor < -1 && getBalanceFactor(retNode->right) <= 0)
			return leftRotate(retNode);

		//RL
		if (balanceFactor < -1 && getBalanceFactor(retNode->right) > 0) {
			cout << "is need RL " << isBalanced() << endl;
			retNode->right = rightRotate(retNode->right);
			return leftRotate(retNode);
		}

		return retNode;
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

	int getHeight(Node* node) {
		if (NULL == node)
			return 0;
		return node->height;
	}

	int getBalanceFactor(Node* node) {
		if (NULL == node) return 0;
		return getHeight(node->left) - getHeight(node->right);
	}

	bool isBalanced(Node* node) {
		if (NULL == node)
			return true;

		int balanceFactor = getBalanceFactor(node);
		if (abs(balanceFactor) > 1)	return false;
		return isBalanced(node->left) && isBalanced(node->right);
	}
public:
	AVLTree()
	{
		root = NULL;
		size = 0;
	}

	~AVLTree()
	{
		if (root)
		{
			Destory(root);
		}
		root = NULL;
	}

	void add(K key, V value) { root = add(root, key, value); }				//add element

	int Size() const { return size; }										//tree size

	bool isBST() {
		list<K> keys;
		inOrder(root, keys);
		auto ite = keys.begin();
		K temp = *ite;
		while (++ite != keys.end()) {
			//temp = *ite;
			if (temp - *ite > 0)
				return false;
			temp = *ite;
		}
		return true;
	}

	bool isEmpty() const { return size == 0; }

	void prePrint() { Print(root); }										//pre scan

	void inOrder() { inOrder(root); }										//inorder scan

	void postOrder() { postOrder(root); }									//post  scan

	void preOrder() { nonPreOrder(root); }									//nonpre sacan

	void levelOrder() { levelOrder(root); }									//deque scan

	bool contains(K key) { return contains(root, key); }					//contains

	bool isBalanced() { return isBalanced(root); }

	K minimum()
	{
		if (0 == size)
			throw - 99;

		return minimum(root)->key;
	}

	K maximum()
	{
		if (0 == size)
			throw - 99;

		return maximum(root)->key;
	}

	K removeMin()
	{
		K ret = minimum();
		root = removeMin(root);
		return ret;
	}

	K removeMax()
	{
		K ret = maximum();
		root = removeMax(root);
		return ret;
	}

	void remove(K key) { root = remove(root, key); }
};

