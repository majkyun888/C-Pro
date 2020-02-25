#pragma once

#include <stack>
#include <vector>
#include "Edge.h"

using namespace std;

template <typename Graph, typename Weight>
class BellmanFord
{
private:
	Graph& G;
	int s;
	Weight* distTo;
	vector<Edge<Weight>*> from;
	bool hasNegativeCycle;

	bool detectNegativeCycle() {

		for (int i = 0; i < G.V(); i++) {
			typename Graph::adjIterator adj(G, i);
			for (Edge<Weight>* e = adj.begin(); !adj.end(); e = adj.next())
				if (from[e->v()] && distTo[e->v()] + e->wt() < distTo[e->w()])
					return true;
		}

		return false;
	}

public:
	BellmanFord(Graph& graph, int s) : G(graph)
	{
		this->s = s;
		distTo = new Weight[G.V()];
		for (int i = 0; i < G.V(); i++)
			from.push_back(NULL);

		from[s] = new Edge<Weight>(s, s, Weight());
		distTo[s] = Weight();

		for (int pass = 1; pass < G.V(); pass++)
		{
			for (int i = 0; i < G.V(); i++)
			{
				typename Graph::adjIterator adj(G, i);
				for (Edge<Weight>* e = adj.begin(); !adj.end(); e = adj.next())
				{
					if (from[e->v()] && (!from[e->w()] || distTo[e->v()] + e->wt() < distTo[e->w()]))
					{
						distTo[e->w()] = distTo[e->v()] + e->wt();
						from[e->w()] = e;
					}
				}
			}
		}
		hasNegativeCycle = detectNegativeCycle();
	}

	~BellmanFord() {

		delete[] distTo;
		delete from[s];
	}

	// 返回图中是否有负权环
	bool negativeCycle() {
		return hasNegativeCycle;
	}

	// 返回从s点到w点的最短路径长度
	Weight shortestPathTo(int w) {
		assert(w >= 0 && w < G.V());
		assert(!hasNegativeCycle);
		assert(hasPathTo(w));
		return distTo[w];
	}

	// 判断从s点到w点是否联通
	bool hasPathTo(int w) {
		assert(w >= 0 && w < G.V());
		return from[w] != NULL;
	}

	// 寻找从s到w的最短路径, 将整个路径经过的边存放在vec中
	void shortestPath(int w, vector<Edge<Weight>>& vec) {

		assert(w >= 0 && w < G.V());
		assert(!hasNegativeCycle);
		assert(hasPathTo(w));

		// 通过from数组逆向查找到从s到w的路径, 存放到栈中
		stack<Edge<Weight>*> s;
		Edge<Weight>* e = from[w];
		while (e->v() != this->s) {
			s.push(e);
			e = from[e->v()];
		}
		s.push(e);

		// 从栈中依次取出元素, 获得顺序的从s到w的路径
		while (!s.empty()) {
			e = s.top();
			vec.push_back(*e);
			s.pop();
		}
	}

	// 打印出从s点到w点的路径
	void showPath(int w) {

		assert(w >= 0 && w < G.V());
		assert(!hasNegativeCycle);
		assert(hasPathTo(w));

		vector<Edge<Weight>> vec;
		shortestPath(w, vec);
		for (int i = 0; i < vec.size(); i++) {
			cout << vec[i].v() << " -> ";
			if (i == vec.size() - 1)
				cout << vec[i].w() << endl;
		}
	}
};