#include <algorithm>
using namespace std;

//Segment tree beats for range min operations and sum/max queries
//Algorithm from https://codeforces.com/blog/entry/57319
//and https://codeforces.com/blog/entry/58564
//Reference implementation: https://github.com/bqi343/USACO/blob/master/Implementations/content/data-structures/1D%20Range%20Queries%20(9.2)/SegTreeBeats.h

constexpr int inf = 1 << 29;

constexpr int maxn = 1 << 20; //set to at least next power of 2

int n, sum[2 * maxn], mx[2 * maxn], mx2[2 * maxn], mxcnt[2 * maxn];

void updatenode(int node)
{
	mx[node] = max(mx[2 * node], mx[2 * node + 1]);
	mx2[node] = max(mx2[2 * node], mx2[2 * node + 1]);
	mxcnt[node] = 0;
	for (int i = 0; i < 2; i++)
	{
		if (mx[2 * node + i] == mx[node])
		{
			mxcnt[node] += mxcnt[2 * node + i];
		}
		else
		{
			mx2[node] = max(mx2[node], mx[2 * node + i]);
		}
	}
	sum[node] = sum[2 * node] + sum[2 * node + 1];
}

void build(int* arr, int node = 1, int nodel = 0, int noder = n - 1)
{
	if (nodel == noder)
	{
		mx[node] = arr[nodel];
		mxcnt[node] = 1;
		mx2[node] = -1;
		sum[node] = arr[nodel];
		return;
	}
	int mid = (nodel + noder) / 2;
	build(arr, 2 * node, nodel, mid);
	build(arr, 2 * node + 1, mid + 1, noder);
	updatenode(node);
}

void lazyprop(int node, int nodel, int noder)
{
	if (nodel == noder)
	{
		return;
	}
	for (int i = 0; i < 2; i++)
	{
		if (mx[2 * node + i] > mx[node])
		{
			sum[2 * node + i] -= mxcnt[2 * node + i] * (mx[2 * node + i] - mx[node]);
			mx[2 * node + i] = mx[node];
		}
	}
}

void update(int l, int r, int val, int node = 1, int nodel = 0, int noder = n - 1)
{
	lazyprop(node, nodel, noder);
	if (noder < l || r < nodel || mx[node] <= val)
	{
		return;
	}
	if (l <= nodel && noder <= r && mx2[node] < val)
	{
		sum[node] -= mxcnt[node] * (mx[node] - val);
		mx[node] = val;
		return;
	}
	int mid = (nodel + noder) / 2;
	update(l, r, val, 2 * node, nodel, mid);
	update(l, r, val, 2 * node + 1, mid + 1, noder);
	updatenode(node);
}

int querysum(int l, int r, int node = 1, int nodel = 0, int noder = n - 1)
{
	if (noder < l || r < nodel)
	{
		return 0;
	}
	lazyprop(node, nodel, noder);
	if (l <= nodel && noder <= r)
	{
		return sum[node];
	}
	int mid = (nodel + noder) / 2;
	return querysum(l, r, 2 * node, nodel, mid) + querysum(l, r, 2 * node + 1, mid + 1, noder);
}

int querymax(int l, int r, int node = 1, int nodel = 0, int noder = n - 1)
{
	if (noder < l || r < nodel)
	{
		return -inf;
	}
	lazyprop(node, nodel, noder);
	if (l <= nodel && noder <= r)
	{
		return mx[node];
	}
	int mid = (nodel + noder) / 2;
	return max(querymax(l, r, 2 * node, nodel, mid), querymax(l, r, 2 * node + 1, mid + 1, noder));
}
