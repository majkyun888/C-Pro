#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include "Edge.h"
#include "indexMinHeap.h"

using namespace std;

template <typename Graph, typename Weight>
class Dijkstra
{
private:
	Graph& G;
	int s;
	Weight* distTo;
	bool* marked;
	vector<Edge<Weight>*> from;
public:
	Dijkstra(Graph& graph, int s) : G(graph)
	{

		this->s = s;
		distTo = new Weight[G.V()];
		marked = new bool[G.V()];
		for (int i = 0; i < G.V(); i++)
		{
			distTo[i] = Weight();
			marked[i] = false;
			from.push_back(NULL);
		}

		IndexMinHeap<Weight> ipq(G.V());
		marked[s] = true;
		distTo[s] = Weight();
		from[s] = new Edge<Weight>(s, s, Weight());
		ipq.insert(s, distTo[s]);

		while (!ipq.isEmpty())
		{
			int v = ipq.extractMinIndex();

			marked[v] = true;

			typename Graph::adjIterator adj(G, v);
			for (Edge<Weight>* e = adj.begin(); !adj.end(); e = adj.next())
			{
				int w = e->other(v);

				if (!marked[w])
				{
					if (from[w] == NULL || distTo[v] + e->wt() < distTo[w])
					{
						from[w] = e;
						distTo[w] = distTo[v] + e->wt();
						if (ipq.contain(w))
						{
							ipq.change(w, distTo[w]);
						}
						else
							ipq.insert(w, distTo[w]);
					}
				}
			}
		}
	}

	~Dijkstra()
	{
		delete marked;
		delete distTo;
		delete from[0];
	}

	bool hasPathTo(int w) const { return marked[w]; }

	Weight shortestPathTo(int w) {
		assert(w >= 0 && w < G.V());
		assert(hasPathTo(w));
		return distTo[w];
	}
	void shortestPath(int w, vector<Edge<Weight>>& vec)
	{
		stack<Edge<Weight>*> s;
		Edge<Weight>* e = from[w];

		while (e->v() != this->s)
		{
			s.push(e);
			e = from[e->v()];
		}
		s.push(e);

		while (!s.empty())
		{
			e = s.top();
			vec.push_back(*e);
			s.pop();
		}
	}

	void showPath(int w)
	{
		vector<Edge<Weight>> vec;
		shortestPath(w, vec);
		for (int i = 0; i < vec.size(); i++)
		{
			cout << vec[i].v() << endl;
			if (i == vec.size() - 1)
				cout << vec[i].w() << endl;
		}
	}
};