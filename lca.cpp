#include <vector>
#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <climits>
using namespace std;

constexpr int maxn = 1005;
constexpr int logn = 25; //MUST BE 1 MORE THAN log2(n)

//Lowest common ancestor

//Based on algorithm from https://wcipeg.com/wiki/lowest_common_ancestor
//and http://wcipeg.com/wiki/Range_minimum_query#Sparse_table

int firstsetbit(int x)
{
#ifndef _MSC_VER
	return sizeof(int) * CHAR_BIT - 1 - __builtin_clz(x);
#else
	unsigned long index;
	_BitScanReverse(&index, x);
	return index;
#endif
}

int n, eorder[maxn], sparse[maxn * 2][logn], depth[maxn];
vector<int> children[maxn];

void adfs(int node = 0)
{
	static int t = 0;
	eorder[node] = t;
	sparse[t++][0] = node;
	for (int child : children[node])
	{
		adfs(child);
		sparse[t++][0] = node;
	}
}

void computeas()
{
	adfs();
	for (int k = 1; k < logn; k++)
	{
		for (int i = 0; i < 2 * n - (1 << k); i++)
		{
			int l = sparse[i][k - 1], r = sparse[i + (1 << (k - 1))][k - 1];
			sparse[i][k] = depth[l] < depth[r] ? l : r;
		}
	}
}

int lca(int node1, int node2)
{
	int pos1 = eorder[node1], pos2 = eorder[node2];
	if (pos1 > pos2)
	{
		swap(pos1, pos2);
	}
	int k = firstsetbit(pos2 - pos1 + 1);
	int l = sparse[pos1][k], r = sparse[pos2 - (1 << k) + 1][k];
	return depth[l] < depth[r] ? l : r;
}
