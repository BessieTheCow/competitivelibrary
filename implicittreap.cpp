#include <iostream>
#include <random>
#include <cstdint>
#include <algorithm>
#include <utility>
#include <tuple>
using namespace std;

//Reference implementation: https://cp-algorithms.com/data_structures/treap.html

constexpr int inf = 1 << 29;

mt19937 rng;

struct node
{
	uint_fast32_t priority = rng();
	int sz = 1;
	int value;
	node* left = nullptr, * right = nullptr;
	long long sum;
	int mx;
	bool rev = false;
	int ad = 0;
	node(int v) : value(v), sum(v), mx(v) {}
};

int sz(node* t)
{
	return t ? t->sz : 0;
}

long long sum(node* t)
{
	return t ? t->sum : 0;
}

int mx(node* t)
{
	return t ? t->mx : -inf;
}

void lazyprop(node* t)
{
	if (!t)
	{
		return;
	}
	if (t->rev)
	{
		if (t->left)
		{
			t->left->rev = !t->left->rev;
		}
		if (t->right)
		{
			t->right->rev = !t->right->rev;
		}
		swap(t->left, t->right);
		t->rev = false;
	}
	if (t->ad)
	{
		if (t->left)
		{
			t->left->ad += t->ad;
		}
		if (t->right)
		{
			t->right->ad += t->ad;
		}
		t->value += t->ad;
		t->sum += static_cast<long long>(t->sz) * t->ad;
		t->mx += t->ad;
		t->ad = 0;
	}
}

void update(node* t)
{
	t->sz = sz(t->left) + sz(t->right) + 1;
	t->sum = sum(t->left) + sum(t->right) + t->value;
	t->mx = max(max(mx(t->left), mx(t->right)), t->value);
}

pair<node*, node*> split(node* t, int key) //split first key nodes
{
	if (!t)
	{
		return { nullptr, nullptr };
	}
	lazyprop(t);
	if (key <= sz(t->left))
	{
		node* l;
		tie(l, t->left) = split(t->left, key);
		lazyprop(t->right);
		update(t);
		return { l, t };
	}
	else
	{
		node* r;
		tie(t->right, r) = split(t->right, key - sz(t->left) - 1);
		lazyprop(t->left);
		update(t);
		return { t, r };
	}
}

node* merge(node* l, node* r)
{
	if (!l)
	{
		lazyprop(r);
		return r;
	}
	if (!r)
	{
		lazyprop(l);
		return l;
	}
	if (l->priority > r->priority)
	{
		lazyprop(l);
		l->right = merge(l->right, r);
		lazyprop(l->left);
		update(l);
		return l;
	}
	else
	{
		lazyprop(r);
		r->left = merge(l, r->left);
		lazyprop(r->right);
		update(r);
		return r;
	}
}

node* root;

int at(node* t, int pos)
{
	lazyprop(t);
	return pos == sz(t->left) ? t->value
		: (pos < sz(t->left) ? at(t->left, pos) : at(t->right, pos - sz(t->left) - 1));
}

int at(int pos)
{
	return at(root, pos);
}

void insert(node*& t, node* u, int pos)
{
	node* lt, * rt;
	tie(lt, rt) = split(t, pos);
	t = merge(merge(lt, u), rt);
}

void insert(int val, int pos)
{
	insert(root, new node(val), pos);
}

void erase(node*& t)
{
	node* oldt = t;
	t = merge(t->left, t->right);
	delete oldt;
}

void erase(node*& t, int pos)
{
	lazyprop(t);
	if (pos == sz(t->left))
	{
		erase(t);
	}
	else if (pos < sz(t->left))
	{
		erase(t->left, pos);
		lazyprop(t->right);
		update(t);
	}
	else
	{
		erase(t->right, pos - sz(t->left) - 1);
		lazyprop(t->left);
		update(t);
	}
}

void erase(int pos)
{
	erase(root, pos);
}

void rev(int l, int r)
{
	node* lt, * mt, * rt;
	tie(lt, rt) = split(root, r + 1);
	tie(lt, mt) = split(lt, l);
	mt->rev = !mt->rev;
	root = merge(merge(lt, mt), rt);
}

void rangeadd(int l, int r, int v)
{
	node* lt, * mt, * rt;
	tie(lt, rt) = split(root, r + 1);
	tie(lt, mt) = split(lt, l);
	mt->ad += v;
	root = merge(merge(lt, mt), rt);
}

long long rangesum(int l, int r)
{
	node* lt, * mt, * rt;
	tie(lt, rt) = split(root, r + 1);
	tie(lt, mt) = split(lt, l);
	long long result = sum(mt);
	root = merge(merge(lt, mt), rt);
	return result;
}

int rangemax(int l, int r)
{
	node* lt, * mt, * rt;
	tie(lt, rt) = split(root, r + 1);
	tie(lt, mt) = split(lt, l);
	int result = mx(mt);
	root = merge(merge(lt, mt), rt);
	return result;
}

void clear(node* t)
{
	if (t)
	{
		clear(t->left);
		clear(t->right);
		delete t;
	}
}

void clear()
{
	clear(root);
	root = nullptr;
}

void siftdown(node* t)
{
	while (true)
	{
		node* mx = t;
		if (t->left && t->left->priority > mx->priority)
		{
			mx = t->left;
		}
		if (t->right && t->right->priority > mx->priority)
		{
			mx = t->right;
		}
		if (mx == t)
		{
			break;
		}
		swap(t->priority, mx->priority);
		t = mx;
	}
}

node* buildsub(int* a, int n)
{
	if (!n)
	{
		return nullptr;
	}
	int mid = n / 2;
	node* t = new node(a[mid]);
	t->left = buildsub(a, mid);
	t->right = buildsub(a + mid + 1, n - mid - 1);
	siftdown(t);
	update(t);
	return t;
}

void build(int* a, int n)
{
	root = buildsub(a, n);
}
