#pragma once
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

template <typename E>
class BSTree
{
private:
	struct Node
	{
		E e;
		Node* left, * right;
		Node(E e)
		{
			this->e = e;
			left = NULL;
			right = NULL;
		}
	};
private:
	Node* root;
	int size;
	/*void add(Node* node, E e)
	{
		if (e == node->e)
			return;
		else if (e < node->e && NULL == node->left)
		{
			node->left = new Node(e);
			size++;
			return;
		}
		else if (e > node->e&& NULL == node->right)
		{
			node->right = new Node(e);
			size++;
			return;
		}

		if (e < node->e)
			add(node->left, e);
		else
			add(node->right, e);
	}*/
	Node* add(Node* node, E e)
	{
		if (NULL == node)
		{
			size++;
			return new Node(e);
		}
		if (e < node->e)
			node->left = add(node->left, e);
		else if (e > node->e)
			node->right = add(node->right, e);
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

	void inOrder(Node* node)
	{
		if (NULL == node)
			return;

		inOrder(node->left);
		cout << node->e << endl;
		inOrder(node->right);
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

	bool contains(Node* node, E e)
	{
		if (NULL == node)
			return false;

		if (e < node->e)
			return contains(node->left, e);
		else if (e > node->e)
			return contains(node->right, e);
		else if (e == node->e)
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

	Node* remove(Node* node, E e)
	{
		if (NULL == node)
			return NULL;

		if (e < node->e)
		{
			node->left = remove(node->left, e);		//5->left = remove(3)
			return node;
		}
		else if (e > node->e)
		{
			node->right = remove(node->right, e);
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

			Node* successor = new Node(minimum(node->right)->e);		//3->right
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
	BSTree()
	{
		root = NULL;
		size = 0;
	}

	~BSTree()
	{
		if (root) Destory(root); root = NULL;
		size = 0;
	}

	/*void add(E e)
	{
		if (NULL == root)
		{
			root = new Node(e);
			size++;
		}
		else
		{
			add(root, e);
		}
	}*/

	void add(E e) { root = add(root, e); }	//add element

	int Size() const { return size; }		//tree size

	bool isEmpty() const { return size == 0; }

	void prePrint() { Print(root); }		//pre scan

	void inOrder() { inOrder(root); }		//inorder scan

	void postOrder() { postOrder(root); }	//post  scan

	void preOrder() { nonPreOrder(root); }	//nonpre sacan

	void levelOrder() { levelOrder(root); }	//deque scan

	bool contains(E e) { return contains(root, e); }		//contains

	E minimum()
	{
		if (0 == size)
			throw - 99;

		return minimum(root)->e;
	}

	E maximum()
	{
		if (0 == size)
			throw - 99;

		return maximum(root)->e;
	}

	E removeMin()
	{
		E ret = minimum();
		root = removeMin(root);
		return ret;
	}

	E removeMax()
	{
		E ret = maximum();
		root = removeMax(root);
		return ret;
	}

	void remove(E e) { root = remove(root, e); }
};


template <typename E>
class BSTSet : public BSTree<E>
{
private:
	BSTree<E> bst;
public:
	int getSize() { return bst.Size(); }
	bool isEmpty() { return bst.isEmpty(); }
	void add(E e) { bst.add(e); }
	bool contains(E e) { bst.contains(e); }
	void remove(E e) { bst.remove(e); }
};


template <typename E>
class LinkSet
{
private:
	list<E> lst;
public:
	int getSize() { return lst.size(); }
	bool isEmpty() { return lst.empty(); }
	void add(E e) { if (!contains(e)) lst.push_front(e); }
	bool contains(E e)
	{
		typename list<E>::iterator ite = find(lst.begin(), lst.end(), e);
		if (ite != lst.end())
			return true;
		else
			return false;
	}
	void remove(E e)
	{
		typename list<E>::iterator ite = find(lst.begin(), lst.end(), e);
		if (ite != lst.end())
		{
			lst.erase(ite);
		}
	}
};