#include <vector>
using namespace std;

constexpr int maxn = 100005;

//Centroid decomposition

vector<int> adj[maxn];
vector<int> children[maxn];
bool visited[maxn];
int parent[maxn];
int sz[maxn];
int root;

void computesize(int node, int p)
{
	sz[node] = 1;
	for (int neighbor : adj[node])
	{
		if (neighbor != p && !visited[neighbor])
		{
			computesize(neighbor, node);
			sz[node] += sz[neighbor];
		}
	}
}

int findcentroid(int node)
{
	computesize(node, -1);
	int s = sz[node];
	int p = -1;
	while (true)
	{
		int maxsize = -1, maxchild = 0;
		for (int neighbor : adj[node])
		{
			if (neighbor != p && !visited[neighbor] && sz[neighbor] > maxsize)
			{
				maxsize = sz[neighbor];
				maxchild = neighbor;
			}
		}
		if (maxsize <= s / 2)
		{
			break;
		}
		p = node;
		node = maxchild;
	}
	return node;
}

int centroiddecomp(int node)
{
	int centroid = findcentroid(node);
	visited[centroid] = true;
	for (int neighbor : adj[centroid])
	{
		if (!visited[neighbor])
		{
			int c = centroiddecomp(neighbor);
			children[centroid].push_back(c);
			parent[c] = centroid;
		}
	}
	return centroid;
}

void centroiddecomp()
{
	root = centroiddecomp(0);
	parent[root] = -1;
}
