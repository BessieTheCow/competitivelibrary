constexpr int maxn = 1005;

//Union-find aka disjoint-set

//Based on algorithm from https://en.wikipedia.org/wiki/Disjoint-set_data_structure

int parent[maxn], r[maxn], sz[maxn], n;

void build()
{
	for (int i = 0; i < n; i++)
	{
		parent[i] = i;
		sz[i] = 1;
	}
}

int find(int node)
{
	if (parent[node] != node)
	{
		parent[node] = find(parent[node]);
	}
	return parent[node];
}

void unite(int a, int b)
{
	a = find(a), b = find(b);
	if (a != b)
	{
		if (r[a] > r[b])
		{
			swap(a, b);
		}
		else if (r[a] == r[b])
		{
			r[b]++;
		}
		parent[a] = b;
		sz[b] += sz[a];
	}
}
