#pragma once
#include <cassert>
#include <vector>

using namespace std;


class DenseGraph
{
private:
	int n, m;
	bool directed;
	vector<vector<bool>> g;
public:
	DenseGraph(int n, bool directed)
	{
		this->n = n;
		this->m = 0;
		this->directed = directed;
		for (int i = 0; i < n; i++)
			g.push_back(vector<bool>(n, false));
	}

	~DenseGraph() {}

	void addEdge(int v, int w)
	{
		if (hasEdge(v, w)) return;

		g[v][w] = true;

		if (!directed) g[w][v] = true;

		m++;
	}
	int V() { return n; }

	int E() { return m; }

	bool hasEdge(int v, int w) { return g[v][w]; }

	void show() const
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				cout << g[i][j] << "\t";
			cout << endl;
		}
	}

	class adjIterator
	{
		private:
			DenseGraph& G;
			int v;
			int index;

		public:
			adjIterator(DenseGraph& graph, int v) : G(graph)
			{
				this->v = v;
				this->index = -1;
			}

			~adjIterator() {}

			int begin()
			{
				index = -1;
				return next();
			}

			int next() {
				for (index += 1; index < G.V(); index++)
					if (G.g[v][index])
						return index;
				return -1;
			}

			bool end() const
			{
				return index >= G.V();
			}

	};

};