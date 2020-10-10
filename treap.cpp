#include <iostream>
#include <random>
#include <cstdint>
using namespace std;

//Treap

//Pages referenced:
//https://cp-algorithms.com/data_structures/treap.html
//https://github.com/bqi343/USACO/blob/master/Implementations/content/data-structures/Treap%20(15.3).h

mt19937_64 gen;

struct node
{
	int key, cnt, sum, dupecnt;
	uint_fast64_t priority;
	node* left, * right;
	node(int k, node* l = nullptr, node* r = nullptr)
		: key(k), cnt(1), sum(k), dupecnt(1), priority(gen()), left(l), right(r) {}
};

node* root = nullptr;

int cnt(node* t)
{
	return t != nullptr ? t->cnt : 0;
}

int sum(node* t)
{
	return t != nullptr ? t->sum : 0;
}

int dupecnt(node* t)
{
	return t != nullptr ? t->dupecnt : 0;
}

void update(node* t)
{
	if (t != nullptr)
	{
		t->cnt = cnt(t->left) + t->dupecnt + cnt(t->right);
		t->sum = sum(t->left) + t->key * t->dupecnt + sum(t->right);
	}
}

void split(node* t, int k, node*& l, node*& r)
{
	if (t == nullptr)
	{
		l = r = nullptr;
	}
	else if (k <= t->key)
	{
		split(t->left, k, l, t->left);
		r = t;
		update(t);
	}
	else
	{
		split(t->right, k, t->right, r);
		l = t;
		update(t);
	}
}

node*& fnd(node*& t, int k)
{
	if (t == nullptr || t->key == k)
	{
		return t;
	}
	else
	{
		return fnd(k < t->key ? t->left : t->right, k);
	}
}

node*& fnd(int k)
{
	return fnd(root, k);
}

void insert(node*& t, node* x)
{
	if (t == nullptr)
	{
		t = x;
		return;
	}
	if (x->priority > t->priority)
	{
		split(t, x->key, x->left, x->right);
		t = x;
	}
	else
	{
		insert(x->key < t->key ? t->left : t->right, x);
	}
	update(t);
}

bool insertdupe(node* t, int x)
{
	if (t == nullptr)
	{
		return false;
	}
	if (t->key == x)
	{
		t->dupecnt++;
		update(t);
		return true;
	}
	if (insertdupe(x < t->key ? t->left : t->right, x))
	{
		update(t);
		return true;
	}
	return false;
}

void insert(int k)
{
	if (!insertdupe(root, k))
	{
		node* t = new node(k);
		insert(root, t);
	}
}

void merge(node* l, node* r, node*& t)
{
	if (l == nullptr)
	{
		t = r;
		return;
	}
	if (r == nullptr)
	{
		t = l;
		return;
	}
	if (l->priority > r->priority)
	{
		merge(l->right, r, l->right);
		t = l;
	}
	else
	{
		merge(l, r->left, r->left);
		t = r;
	}
	update(t);
}

void erase(node*& t, int x)
{
	if (t != nullptr)
	{
		if (t->key == x)
		{
			t->dupecnt--;
			if (t->dupecnt == 0)
			{
				node* temp = t;
				merge(t->left, t->right, t);
				delete temp;
			}
			else
			{
				update(t);
			}
		}
		else
		{
			erase(x < t->key ? t->left : t->right, x);
			update(t);
		}
	}
}

void erase(int x)
{
	erase(root, x);
}

void clear(node* t)
{
	if (t != nullptr)
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

node*& at(node*& t, int i)
{
	if (t == nullptr)
	{
		return t;
	}
	int lc = cnt(t->left);
	return i >= lc && i < lc + t->dupecnt ? t : (lc > i ? at(t->left, i) : at(t->right, i - lc - t->dupecnt));
}

node*& at(int i)
{
	return at(root, i);
}

int cntless(node* t, int x)
{
	if (t == nullptr)
	{
		return 0;
	}
	else if (x < t->key)
	{
		return cntless(t->left, x);
	}
	else if (x == t->key)
	{
		return cnt(t->left);
	}
	else
	{
		return cnt(t->left) + t->dupecnt + cntless(t->right, x);
	}
}

int cntless(int x)
{
	return cntless(root, x);
}

void siftdown(node* t)
{
	if (t != nullptr)
	{
		if (t->left != nullptr && t->left->priority > t->priority && (t->right == nullptr || t->left->priority > t->right->priority))
		{
			swap(t->left->priority, t->priority);
			siftdown(t->left);
		}
		else if (t->right != nullptr && t->right->priority > t->priority)
		{
			swap(t->right->priority, t->priority);
			siftdown(t->right);
		}
	}
}

node* build(int* begin, int* end)
{
	if (begin == end)
	{
		return nullptr;
	}
	int* mid = begin + (end - begin) / 2;
	node* t = new node(*mid, build(begin, mid), build(mid + 1, end));
	siftdown(t);
	update(t);
	return t;
}

void print(node* t = root)
{
	if (t != nullptr)
	{
		print(t->left);
		for (int i = 0; i < t->dupecnt; i++)
		{
			cout << t->key << '\n';
		}
		print(t->right);
	}
}
