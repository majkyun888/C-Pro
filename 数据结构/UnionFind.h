#pragma once 
#include <vector>
using namespace std;


struct UF
{
	virtual int getSize() = 0;
	virtual bool isConnected(int p, int q) = 0;
};


class UnionFind1 : public UF
{
private:
	vector<int> vec_union;

	int find(int p) {
		if (p < 0 || p >= vec_union.size())
			throw - 99;
		return vec_union.at(p);
	}
public:
	UnionFind1(int size) {
		for (int i = 0; i < size; i++) vec_union.push_back(i);
	}

	int getSize() { return vec_union.size(); }

	bool isConnected(int p, int q) { return find(p) == find(q); }

	void unionElements(int p, int q) {
		int pID = find(p);
		int qID = find(q);

		if (pID == qID)
			return;

		for (int i = 0; i < vec_union.size(); i++)
			if (vec_union[i] == pID)
				vec_union[i] = qID;
	}
};


class UnionFind2 : public UF
{
private:
	vector<int> parent;
	int find(int p) {
		if (p < 0 || p >= parent.size())
			throw - 99;

		while (p != parent[p]) p = parent[p];
		return parent[p];
	}
public:
	UnionFind2(int size) { for (int i = 0; i < size; i++) parent.push_back(i); }

	int getSize() { return parent.size(); }

	bool isConnected(int p, int q) { return find(p) == find(q); }

	void unionElements(int p, int q) {
		int pRoot = find(p);
		int qRoot = find(q);

		if (pRoot == qRoot) return;

		parent[pRoot] = qRoot;
	}
};


class UnionFind3 : public UF
{
private:
	vector<int> sz;
	vector<int> parent;
	
	int find(int p) {
		if (p < 0 || p >= parent.size())
			throw - 99;

		while (p != parent[p]) p = parent[p];
		
		return parent[p];
	}
public:
	UnionFind3(int size) {
		for (int i = 0; i < size; i++) {
			parent.push_back(i);
			sz.push_back(1);
		}
	}

	int getSize() { return parent.size(); }

	bool isConnected(int p, int q) { return find(p) == find(q); }

	void unionElements(int p, int q) {
		int pRoot = find(p);
		int qRoot = find(q);

		if (pRoot == qRoot)
			return;

		if (sz[pRoot] > sz[qRoot]) {
			parent[qRoot] = pRoot;
			sz[pRoot] += sz[qRoot];
		}
		else
		{
			parent[pRoot] = qRoot;
			sz[qRoot] += sz[pRoot];
		}
	}
};


class UnionFind4 : public UF
{
private:
	vector<int> parent;
	vector<int> rank;

	int find(int p)
	{
		if (p < 0 || p >= parent.size())
			throw - 99;

		while (p != parent[p]) p = parent[p];

		return parent[p];
	}
public:
	UnionFind4(int size) {
		for (int i = 0; i < size; i++) {
			parent.push_back(i);
			rank.push_back(1);
		}
	}

	int getSize() { return parent.size(); }

	bool isConnected(int p, int q) { return find(p) == find(q); }

	void unionElements(int p, int q) {
		int pRoot = find(p);
		int qRoot = find(q);

		if (pRoot == qRoot)
			return;

		if (rank[pRoot] < rank[qRoot])
			parent[pRoot] = qRoot;
		else if (rank[pRoot] > rank[qRoot])
			parent[qRoot] = pRoot;
		else
		{
			parent[pRoot] = qRoot;
			rank[qRoot] += 1;
		}
	}
};


class UnionFind5 : public UF
{
private:
	vector<int> rank;
	vector<int> parent;

	int find(int p) {

		if (p < 0 || p >= parent.size())
			throw - 99;

		while (p != parent[p]) {
			parent[p] = parent[parent[p]];
			p = parent[p];
		}

		return p;
	}
public:
	UnionFind5(int size) {
		for (int i = 0; i < size; i++) {
			parent.push_back(i);
			rank.push_back(1);
		}
	}

	int getSize() { return parent.size(); }

	bool isConnected(int p, int q) { return find(p) == find(q); }

	void unionElements(int p, int q) {
		
		int pRoot = find(p);
		int qRoot = find(q);

		if (pRoot == qRoot)
			return;

		if (rank[pRoot] < rank[qRoot])
			parent[pRoot] = qRoot;
		else if (rank[pRoot] > rank[qRoot])
			parent[qRoot] = pRoot;
		else
		{
			parent[pRoot] = qRoot;
			rank[qRoot] += 1;
		}
	}
};


class UnionFind6 : public UF
{
private:
	vector<int> rank;
	vector<int> parent;
	int find(int p) {
		if (p < 0 || p >= parent.size())
			throw - 99;

		if (p != parent[p]) { parent[p] = find(parent[p]); }
		return parent[p];
	}
public:
	UnionFind6(int size) {
		for (int i = 0; i < size; i++) {
			parent.push_back(i);
			rank.push_back(1);
		}
	}

	int getSize() { return parent.size(); }

	bool isConnected(int p, int q) { return find(p) == find(q); }

	void unionElements(int p, int q) {

		int pRoot = find(p);
		int qRoot = find(q);

		if (pRoot == qRoot)
			return;

		if (rank[pRoot] < rank[qRoot]) parent[pRoot] = qRoot;
		else if (rank[pRoot] > rank[qRoot]) parent[qRoot] = pRoot;
		else {
			parent[pRoot] = qRoot;
			rank[qRoot] += 1;
		}
	}
};