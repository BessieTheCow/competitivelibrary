constexpr int maxn = 1005;

//Binary indexed tree aka Fenwick tree template
//Increment indices for 0 indexing, querying index 0 gives 0
//Do not update index 0
//Copy y when doing 2D

int bit[maxn], n;

void update(int index, int val)
{
	for (; index <= n; index += index & -index)
	{
		bit[index] += val;
	}
}

int query(int index)
{
	int sum = 0;
	for (; index > 0; index -= index & -index)
	{
		sum += bit[index];
	}
	return sum;
}
