#include <iostream>
#include <algorithm>
using namespace std;

constexpr int logs = 20;
constexpr int inf = 1 << logs;

//Binary trie
//Based on https://threads-iiith.quora.com/Tutorial-on-Trie-and-example-problems
//and https://codeforces.com/blog/entry/59462

struct node
{
	int cnt, minval;
	node* child[2];
	node(int value, int c = 0, node * leftchild = nullptr, node * rightchild = nullptr)
		: minval(value), cnt(c), child{ leftchild, rightchild } {}
};

node root(inf);

int cnt(node* t)
{
	return t != nullptr ? t->cnt : 0;
}

int minval(node* t)
{
	return t != nullptr ? t->minval : inf;
}

void update(node& t)
{
	t.cnt = cnt(t.child[0]) + cnt(t.child[1]);
	t.minval = min(minval(t.child[0]), minval(t.child[1]));
}

void insert(node& t, int mask, int x)
{
	if (mask == 0)
	{
		t.cnt++;
		return;
	}
	bool bit = x & mask;
	if (t.child[bit] == nullptr)
	{
		t.child[bit] = new node(x);
	}
	insert(*t.child[bit], mask >> 1, x);
	update(t);
}

void insert(int x)
{
	insert(root, 1 << (logs - 1), x);
}

void erase(node& t, int mask, int x)
{
	if (mask == 0)
	{
		t.cnt--;
		return;
	}
	bool bit = x & mask;
	if (t.child[bit] != nullptr)
	{
		erase(*t.child[bit], mask >> 1, x);
		if (t.child[bit]->cnt == 0)
		{
			delete t.child[bit];
			t.child[bit] = nullptr;
		}
		update(t);
	}
}

void erase(int x)
{
	erase(root, 1 << (logs - 1), x);
}

int fnd(int x)
{
	node* t = &root;
	for (int mask = 1 << (logs - 1); mask; mask >>= 1)
	{
		bool bit = x & mask;
		if (t->child[bit] == nullptr)
		{
			return 0;
		}
		else
		{
			t = t->child[bit];
		}
	}
	return t->cnt;
}

void print(node& t = root, int x = 0, int d = 0)
{
	if (d == logs)
	{
		for (int i = 0; i < t.cnt; i++)
		{
			cout << x << '\n';
		}
	}
	else
	{
		if (t.child[0] != nullptr)
		{
			print(*t.child[0], x << 1, d + 1);
		}
		if (t.child[1] != nullptr)
		{
			print(*t.child[1], (x << 1) | 1, d + 1);
		}
	}
}

int maxxor(int x, int mx = inf - 1)
{
	if (root.minval > mx)
	{
		return -inf;
	}
	int result = 0;
	node* t = &root;
	for (int mask = 1 << (logs - 1); mask; mask >>= 1)
	{
		bool bit = x & mask;
		result <<= 1;
		if (minval(t->child[!bit]) <= mx)
		{
			result |= static_cast<int>(!bit);
			t = t->child[!bit];
		}
		else
		{
			result |= static_cast<int>(bit);
			t = t->child[bit];
		}
	}
	return result;
}

int cntxorless(int x, int mx)
{
	int total = 0;
	node* t = &root;
	for (int mask = 1 << (logs - 1); mask; mask >>= 1)
	{
		bool bit = x & mask;
		if (mx & mask)
		{
			total += cnt(t->child[bit]);
			t = t->child[!bit];
		}
		else
		{
			t = t->child[bit];
		}
		if (t == nullptr)
		{
			break;
		}
	}
	return total;
}

int cntless(int mx)
{
	return cntxorless(0, mx);
}

void clear(node& t)
{
	if (t.child[0] != nullptr)
	{
		clear(*t.child[0]);
		delete t.child[0];
	}
	if (t.child[1] != nullptr)
	{
		clear(*t.child[1]);
		delete t.child[1];
	}
}

void clear()
{
	clear(root);
	root.child[0] = nullptr;
	root.child[1] = nullptr;
}
