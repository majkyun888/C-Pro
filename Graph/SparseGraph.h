#pragma once
#include <vector>
using namespace std;

class SparseGraph
{
private:
	int n, m;
	bool directed;
	vector<vector<int>> g;
public:
	SparseGraph(int n, bool directed)
	{
		this->n = n;
		this->m = 0;
		this->directed = directed;

		g = vector<vector<int>>(n, vector<int>());
	}

	~SparseGraph() {}

	int V() { return n; }
	
	int E() { return m; }

	void addEdge(int v, int w)
	{
		g[v].push_back(w);
		if (w != v && !directed)
			g[w].push_back(v);
		m++;
	}

	bool hasEdge(int v, int w)
	{
		for (int i = 0; i < g[v].size(); i++)
			if (g[v][i] == w)
				return true;
		return false;
	}

	void show() const
	{
		for (int i = 0; i < n; i++)
		{
			cout << "vertex " << i << ":\t";
			for (int j = 0; j < g[i].size(); j++)
				cout << g[i][j] << "\t";
			cout << endl;
		}
	}

	class adjIterator {
		private:
			SparseGraph& G;
			int v;
			int index;
		public:
			adjIterator(SparseGraph& graph, int v) : G(graph)
			{
				this->v = v;
				this->index = 0;
			}

			~adjIterator() {}

			int begin() 
			{
				index = 0;
				if (G.g[v].size())	return G.g[v][index];
			}

			int next() 
			{
				index++;
				if (index < G.g[v].size())
					return G.g[v][index];

				return -1;
			}

			bool end() const
			{
				return index >= G.g[v].size();
			}
	};
};