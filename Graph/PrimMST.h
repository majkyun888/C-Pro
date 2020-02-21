#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include "indexMinHeap.h"
#include "Edge.h"

using namespace std;


template <typename Graph, typename Weight>
class PrimMST
{
private:
	Graph& G;
	IndexMinHeap<Weight> ipq;						//
	vector<Edge<Weight>*> edgeTo;
	bool* marked;
	vector<Edge<Weight>> mst;
	Weight mstWeight;

	void visit(int v)
	{
		marked[v] = true;

		typename Graph::adjIterator adj(G, v);
		for (Edge<Weight>* e = adj.begin(); !adj.end(); e = adj.next())
		{
			int w = e->other(v);
			if (!marked[w])
			{
				if (!edgeTo[w])
				{
					edgeTo[w] = e;
					ipq.insert(w, e->wt());
				}
				else if (e->wt() < edgeTo[w]->wt())
				{
					edgeTo[w] = e;
					ipq.change(w, e->wt());
				}
			}
		}
	}
public:
	PrimMST(Graph& graph) : G(graph), ipq(IndexMinHeap<double>(graph.V()))
	{
		marked = new bool[G.V()];
		for (int i = 0; i < G.V(); i++)
		{
			marked[i] = false;
			edgeTo.push_back(NULL);
		}

		mst.clear();
		visit(0);
		while (!ipq.isEmpty())
		{
			int w = ipq.extractMinIndex();
			mst.push_back(*edgeTo[w]);
			visit(w);
		}

		mstWeight = mst[0].wt();
		for (int i = 1; i < mst.size(); i++)
			mstWeight += mst[i].wt();
	}

	~PrimMST()
	{
		delete[] marked;
	}

	vector<Edge<Weight>> mstEdges()
	{
		return mst;
	}

	Weight result() const { return mstWeight; }
};
