constexpr int maxn = 1005;

//Binary indexed tree with range update and range query
//1 indexed, increment indices for 0 index
//querying index 0 gives 0, do not update index 0

//Sources referenced:
//https://www.geeksforgeeks.org/binary-indexed-tree-range-update-range-queries/
//https://github.com/bqi343/USACO/blob/master/Implementations/09%20-%20Range%20Queries%20(2)/09.2%20-%201D%20Range%20Queries%20(3)/Binary%20Indexed%20Tree.cpp
//Alphastar Academy CS501AB: USACO Platinum Student's Book

int bit[2][maxn], n; //1 index both array and tree, function must be associative

void update(int* tree, int index, int val) // adds val to index in underlying bit
{
	for (; index <= n; index += index & -index)
	{
		tree[index] += val;
	}
}

int query(int* tree, int index) //returns sum of elements 1...index aka first index elements in underlying bit
{
	int sum = 0;
	for (; index > 0; index -= index & -index)
	{
		sum += tree[index];
	}
	return sum;
}

void update(int left, int right, int val) //adds val to all elements in [left, right]
{
	update(bit[0], left, -val * (left - 1));
	update(bit[0], right + 1, val * right);
	update(bit[1], left, val);
	update(bit[1], right + 1, -val);
}

int query(int index) //returns sum of elements 1...index aka first index elements
{
	return query(bit[0], index) + index * query(bit[1], index);
}

int query(int left, int right) //returns sum of elements in [left, right]
{
	return query(right) - query(left - 1);
}
