#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

//Complexity: O(V^2*sqrt(E))

constexpr int inf = 1 << 29;

constexpr int maxn = 5005;

//Reference implementation: https://github.com/kth-competitive-programming/kactl/blob/master/content/graph/PushRelabel.h

struct edge
{
	int to, rev, flow, capacity;
};

vector<edge> g[maxn];
int n, s, t;
int excess[maxn], height[maxn], cur[maxn];
vector<int> active[2 * maxn];
int highestactive;
int heightcnt[2 * maxn];

void addedge(int u, int v, int c, int rc = 0)
{
	g[u].push_back({ v, g[v].size(), 0, c });
	g[v].push_back({ u, static_cast<int>(g[u].size()) - 1, 0, rc });
}

void addflow(edge& e, int f)
{
	edge& rev = g[e.to][e.rev];
	assert(f <= e.capacity);
	excess[rev.to] -= f;
	e.flow += f;
	e.capacity -= f;
	excess[e.to] += f;
	rev.flow -= f;
	rev.capacity += f;
}

void push(edge& e)
{
	int v = e.to;
	int u = g[v][e.rev].to;
	assert(height[u] == height[v] + 1);
	addflow(e, min(excess[u], e.capacity));
}

void relabel(int u)
{
	height[u] = inf;
	for (edge& v : g[u])
	{
		if (v.capacity && height[v.to] + 1 < height[u])
		{
			height[u] = height[v.to] + 1;
		}
	}
}

void gap(int gapheight)
{
	for (int i = 0; i < n; i++)
	{
		if (height[i] > gapheight && height[i] < n)
		{
			heightcnt[height[i]]--;
			height[i] = n + 1;
		}
	}
}

void discharge(int u)
{
	while (excess[u])
	{
		if (cur[u] == static_cast<int>(g[u].size()))
		{
			relabel(u);
			cur[u] = 0;
			heightcnt[height[u]]++;
			heightcnt[highestactive]--;
			if (heightcnt[highestactive] == 0)
			{
				gap(highestactive);
			}
			highestactive = height[u];
		}
		else if (height[g[u][cur[u]].to] < height[u] && g[u][cur[u]].capacity)
		{
			if (excess[g[u][cur[u]].to] == 0 && g[u][cur[u]].to != t)
			{
				active[height[u] - 1].push_back(g[u][cur[u]].to);
			}
			push(g[u][cur[u]]);
		}
		else
		{
			cur[u]++;
		}
	}
}

int maxflow()
{
	height[s] = n;
	heightcnt[0] = n - 1;
	for (edge& e : g[s])
	{
		addflow(e, e.capacity);
		if (e.to != t)
		{
			active[0].push_back(e.to);
		}
	}
	while (true)
	{
		while (active[highestactive].empty())
		{
			if (highestactive == 0)
			{
				return excess[t];
			}
			highestactive--;
		}
		int u = active[highestactive].back();
		active[highestactive].pop_back();
		discharge(u);
	}
}

bool mincutleft(int u)
{
	return height[u] >= n;
}
