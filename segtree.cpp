constexpr int maxn = 1024; //set to at least the next power of 2

//Segment tree with lazy propapgation

//Pages referenced:
//https://www.hackerearth.com/practice/data-structures/advanced-data-structures/segment-trees/tutorial/
//https://www.hackerearth.com/practice/notes/segment-tree-and-lazy-propagation/
//https://github.com/bqi343/USACO/blob/master/Implementations/content/data-structures/1D%20Range%20Queries%20(9.2)/LazySeg%20(15.2).h

int tree[2 * maxn], lazy[2 * maxn], n; //maxn must be greater than the next power of 2

int f(int a, int b)
{
	return a + b;
}

void updatenode(int node)
{
	tree[node] = f(tree[2 * node], tree[2 * node + 1]);
}

constexpr int identity = 0;

void change(int& orig, int cnt, int val)
{
	orig += cnt * val;
}

void build(int* arr, int node = 1, int nodel = 0, int noder = n - 1)
{
	if (nodel == noder)
	{
		tree[node] = arr[nodel];
	}
	else
	{
		int mid = (nodel + noder) / 2;
		build(2 * node, nodel, mid);
		build(2 * node + 1, mid + 1, noder);
		updatenode(node);
	}
}

void lazyprop(int node, int nodel, int noder)
{
	change(tree[node], noder - nodel + 1, lazy[node]);
	if (nodel < noder)
	{
		lazy[2 * node] += lazy[node];
		lazy[2 * node + 1] += lazy[node];
	}
	lazy[node] = 0;
}

void update(int index, int val, int node = 1, int nodel = 0, int noder = n - 1)
{
	lazyprop(node, nodel, noder);
	if (nodel == noder)
	{
		arr[index] += val;
		tree[node] += val;
	}
	else
	{
		int mid = (nodel + noder) / 2;
		if (index <= mid)
		{
			update(index, val, 2 * node, nodel, mid);
		}
		else
		{
			update(index, val, 2 * node + 1, mid + 1, noder);
		}
		updatenode(node);
	}
}

void updaterange(int left, int right, int val, int node = 1, int nodel = 0, int noder = n - 1)
{
	lazyprop(node, nodel, noder);
	if (nodel > right || noder < left)
	{
		return;
	}
	if (nodel >= left && noder <= right)
	{
		lazy[node] = val;
		lazyprop(node, nodel, noder);
		return;
	}
	int mid = (nodel + noder) / 2;
	updaterange(left, right, val, node * 2, nodel, mid);
	updaterange(left, right, val, node * 2 + 1, mid + 1, noder);
	updatenode(node);
}

int query(int left, int right, int node = 1, int nodel = 0, int noder = n - 1)
{
	if (right < nodel || noder < left)
	{
		return identity;
	}
	lazyprop(node, nodel, noder);
	if (left <= nodel && noder <= right)
	{
		return tree[node];
	}
	int mid = (nodel + noder) / 2;
	return f(query(left, right, 2 * node, nodel, mid), query(left, right, 2 * node + 1, mid + 1, noder));
}
