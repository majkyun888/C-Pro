#pragma once
#include <vector>
#include <queue>
#include <iostream>
#include <stack>

using namespace std;

template <typename Graph>
class ShortestPath
{
private:
	Graph& G;
	int s;
	bool* visited;
	int* from;
	int* ord;
public:
	ShortestPath(Graph& graph, int s) : G(graph)
	{
		visited = new bool[graph.V()];
		from = new int[graph.V()];
		ord = new int[graph.V()];

		for (int i = 0; i < graph.V(); i++)
		{
			visited[i] = false;
			from[i] = -1;
			ord[i] = -1;
		}

		this->s = s;

		queue<int> q;

		q.push(s);
		visited[s] = true;
		ord[s] = 0;

		while (!q.empty())
		{
			int v = q.front();
			q.pop();

			typename Graph::adjIterator adj(G, v);
			for (int i = adj.begin(); !adj.end(); i = adj.next())
			{
				if (!visited[i])
				{
					q.push(i);
					visited[i] = true;
					from[i] = v;
					ord[i] = ord[v] + 1;
				}
			}
		}
	}

	~ShortestPath()
	{
		delete[] from;
		delete[] visited;
		delete[] ord;
	}

	bool hasPath(int w) const
	{
		return visited[w];
	}

	void path(int w, vector<int>& vec)
	{
		stack<int> s;
		int p = w;
		while (p != -1)
		{
			s.push(p);
			p = from[p];
		}

		vec.clear();
		while (!s.empty())
		{
			vec.push_back(s.top());
			s.pop();
		}
	}

	void showPath(int w)
	{
		vector<int> vec;
		path(w, vec);
		for (int i = 0; i < vec.size(); i++)
		{
			cout << vec[i];
			if (i == vec.size() - 1)
				cout << endl;
			else
				cout << "->";
		}
	}

	int length(int w)
	{
		return ord[w];
	}



};