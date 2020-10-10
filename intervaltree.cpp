#include <utility>
#include <set>
#include <cassert>
#include <tuple>
#include <vector>
#include <limits>
using namespace std;

//O(log n) insert/erase, O(log n + m) query where m is number of results
//To query for intervals intersecting a given range, use separate list of endpoints
//to find intervals with at least one endpoint in the range, then query for an arbitraty point
//in the range.

constexpr int lo = -1000000000, hi = 1000000000;
constexpr int inf = numeric_limits<int>::max(), neginf = numeric_limits<int>::min();

int mid(int a, int b) //midpoint without overflow
{
	return (a > 0) == (b > 0) ? a + (b - a) / 2 : (a + b) / 2;
}

bool compright(const pair<pair<int, int>, int>& a, const pair<pair<int, int>, int>& b)
{
	return a.first.second != b.first.second ? a.first.second < b.first.second
		: (a.first.first != b.first.first ? a.first.first < b.first.first : a.second < b.second);
}

struct node
{
	multiset<pair<pair<int, int>, int>> sl;
	multiset<pair<pair<int, int>, int>, decltype(&compright)> sr{ compright };
	node* left = nullptr, * right = nullptr;
};

node* root;

void insert(const pair<pair<int, int>, int>& x, node*& t = root, int l = lo, int r = hi)
{
	assert(x.first.first >= l && x.first.second <= r);
	if (!t)
	{
		t = new node;
	}
	int c = mid(l, r);
	if (x.first.second < c)
	{
		insert(x, t->left, l, c - 1);
	}
	else if (x.first.first > c)
	{
		insert(x, t->right, c + 1, r);
	}
	else
	{
		t->sl.insert(x);
		t->sr.insert(x);
	}
}

void erase(const pair<pair<int, int>, int>& x, node*& t = root, int l = lo, int r = hi)
{
	assert(x.first.first >= l && x.first.second <= r);
	if (!t)
	{
		return;
	}
	int c = mid(l, r);
	if (x.first.second < c)
	{
		erase(x, t->left, l, c - 1);
	}
	else if (x.first.first > c)
	{
		erase(x, t->right, l, c + 1);
	}
	else
	{
		t->sl.erase(x);
		t->sr.erase(x);
	}
	if (t->sl.empty() && !t->left && !t->right)
	{
		delete t;
		t = nullptr;
	}
}

void clear(node* t)
{
	if (!t)
	{
		return;
	}
	clear(t->left);
	clear(t->right);
	delete t;
}

void clear()
{
	clear(root);
	root = nullptr;
}

void query(int x, vector<int>& result, node* t = root, int l = lo, int r = hi)
{
	if (!t)
	{
		return;
	}
	int c = mid(l, r);
	if (x < c)
	{
		auto end = t->sl.upper_bound(make_pair(make_pair(x, inf), inf));
		for (auto it = t->sl.begin(); it != end; ++it)
		{
			result.push_back(it->second);
		}
		query(x, result, t->left, l, c - 1);
	}
	else if (x > c)
	{
		auto start = t->sr.lower_bound(make_pair(make_pair(neginf, x), neginf));
		for (auto it = start; it != t->sr.end(); ++it)
		{
			result.push_back(it->second);
		}
		query(x, result, t->right, c + 1, r);
	}
	else
	{
		for (auto& i : t->sl)
		{
			result.push_back(i.second);
		}
	}
}
