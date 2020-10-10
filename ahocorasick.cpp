#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <vector>
#include <cassert>
using namespace std;

//Complexity: Linear in number of matches

constexpr int alphsz = 26;

int m;
string w[1005];

struct node
{
	node* nxt[alphsz] = { nullptr };
	node* sufflink = nullptr, * dictlink = nullptr;
	int leaf = -1;
};

node root;
vector<node*> nodes;

void buildtrie()
{
	for (int i = 0; i < m; i++)
	{
		node* curr = &root;
		for (char c : w[i])
		{
			int v = c - 'a';
			if (!curr->nxt[v])
			{
				curr->nxt[v] = new node;
				nodes.push_back(curr->nxt[v]);
			}
			curr = curr->nxt[v];
		}
		assert(curr->leaf == -1);
		curr->leaf = i;
	}
}

void buildlinks()
{
	queue<node*> q;
	q.push(&root);
	while (!q.empty())
	{
		node* curr = q.front();
		q.pop();
		for (int i = 0; i < alphsz; i++)
		{
			if (curr->nxt[i])
			{
				curr->nxt[i]->sufflink = curr == &root ? &root : curr->sufflink->nxt[i];
				q.push(curr->nxt[i]);
			}
		}
		if (curr != &root)
		{
			for (int i = 0; i < alphsz; i++)
			{
				if (!curr->nxt[i])
				{
					curr->nxt[i] = curr->sufflink->nxt[i];
				}
			}
			curr->dictlink = curr->sufflink->leaf != -1 ? curr->sufflink : curr->sufflink->dictlink;
		}
		else
		{
			for (int i = 0; i < alphsz; i++)
			{
				if (!curr->nxt[i])
				{
					curr->nxt[i] = &root;
				}
			}
		}
	}
}

vector<pair<int, int>> strsearch(const string& text)
{
	vector<pair<int, int>> r;
	node* curr = &root;
	for (int i = 0; i < (int)text.size(); i++)
	{
		int v = text[i] - 'a';
		curr = curr->nxt[v];
		if (curr->leaf != -1)
		{
			r.emplace_back(curr->leaf, i - (int)w[curr->leaf].size() + 1);
		}
		node* t = curr->dictlink;
		while (t)
		{
			assert(t->leaf != -1);
			r.emplace_back(t->leaf, i - (int)w[t->leaf].size() + 1);
			t = t->dictlink;
		}
	}
	return r;
}

void clear()
{
	for (node* nd : nodes)
	{
		delete nd;
	}
}
