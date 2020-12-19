#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <numeric>
#include <queue>
using namespace std;

//Complexity: O(N^3)?

//Reference implementations:
//https://github.com/bqi343/USACO/blob/master/Implementations/content/graphs%20(12)/Matching/UnweightedMatch.h
//https://github.com/koosaga/DeobureoMinkyuParty

mt19937 rng;

constexpr int maxn = 505;

int n;
vector<int> adj[maxn];
int match[maxn]; //mate of a vertex, or -1 if unmatched
int parent[maxn]; //parent in the BFS tree, indicates alternating path to unmatched vertex
int visited[maxn]; //-1 if unvisited, 0 if even, 1 if odd; set when vertex is first reached
int tip[maxn]; //tip of the blossom which a vertex is part of
int aux[maxn]; //used by lca to remember visited vertices

void augment(int v) //toggle edges
{
	while (v != -1)
	{
		int nv = match[parent[v]];
		match[v] = parent[v];
		match[parent[v]] = v;
		v = nv;
	}
}

int lca(int v, int w) //find tip of blossom
{
	static int t = 0;
	t++;
	while (true)
	{
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6385 )
#pragma warning( disable : 6386 )
#endif
		if (aux[v] == t)
		{
			return v;
		}
		aux[v] = t;
		v = match[v] != -1 ? tip[parent[match[v]]] : -1;
#ifdef _MSC_VER
#pragma warning( pop )
#endif
		if (w != -1)
		{
			swap(v, w);
		}
	}
}

void blossom(int v, int w, int a, queue<int>& q) //contract one side of blossom
{
	while (tip[v] != a)
	{
		parent[v] = w;
		w = match[v];
		visited[w] = 0;
		q.push(w);
		tip[v] = a;
		tip[w] = a;
		v = parent[w];
	}
}

bool bfs(int u) //find augmenting path and augment
{
	parent[u] = -1;
	fill_n(visited, n, -1);
	iota(tip, tip + n, 0);
	queue<int> q;
	q.push(u);
	visited[u] = 0;
	while (!q.empty())
	{
		int v = q.front();
		q.pop();
		for (int x : adj[v])
		{
			if (visited[x] == -1)
			{
				parent[x] = v;
				if (match[x] == -1)
				{
					augment(x);
					return true;
				}
				visited[x] = 1;
				visited[match[x]] = 0;
				q.push(match[x]);
			}
			else if (visited[x] == 0 && tip[x] != tip[v])
			{
				int a = lca(tip[x], tip[v]);
				blossom(v, x, a, q);
				blossom(x, v, a, q);
			}
		}
	}
	return false;
}
