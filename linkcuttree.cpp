#include <vector>
#include <cassert>
using namespace std;

//fully dynamic connectivity on acyclic graphs
//maintains forest of rooted trees
//can be augmented to allow path queries
//lca queries can be done by accessing the two nodes, splaying the first node and taking the
//pathparent pointer

//complexity: O(log n) amortized

//algorithm description: https://en.wikipedia.org/wiki/Link%2Fcut_tree
//reference implementations:
//https://github.com/kth-competitive-programming/kactl/blob/master/content/graph/LinkCutTree.h
//https://codeforces.com/blog/entry/75885

struct node
{
	node* parent = nullptr, * pathparent = nullptr;
	node* child[2] = { nullptr, nullptr };
	bool rev = false; //lazy propagaing tag for reversing paths in order to reroot
	void proprev() //propagate the reverse tag
	{
		if (rev)
		{
			rev = false;
			swap(child[0], child[1]);
			if (child[0])
			{
				child[0]->rev = !child[0]->rev;
			}
			if (child[1])
			{
				child[1]->rev = !child[1]->rev;
			}
		}
	}
	void rot(bool c) //rotate with child c
	{
		assert(child[c]);
		if (parent)
		{
			parent->child[whichchild()] = child[c];
		}
#ifdef _MSC_VER
#pragma warning(suppress: 6011)
#endif // _MSC_VER
		node* mid = child[c]->child[!c];
		child[c]->parent = parent;
		child[c]->child[!c] = this;
		parent = child[c];
		child[c] = mid;
		if (mid)
		{
			mid->parent = this;
		}
	}
	bool whichchild()
	{
		assert(parent);
#ifdef _MSC_VER
#pragma warning(suppress: 6011)
#endif // _MSC_VER
		return parent->child[1] == this;
	}
	void splay() //move to root of the splay tree, propagates reverse tags
	{
		proprev();
		while (parent)
		{
			if (!parent->parent) //zig
			{
				parent->proprev();
				proprev();
				pathparent = parent->pathparent;
				parent->rot(whichchild());
				break;
			}
			else
			{
				parent->parent->proprev();
				parent->proprev();
				proprev();
				if (!parent->parent->parent)
				{
					pathparent = parent->parent->pathparent;
				}
				if (whichchild() == parent->whichchild()) //zig-zig
				{
					parent->parent->rot(parent->whichchild());
					parent->rot(whichchild());
				}
				else //zig-zag
				{
					parent->rot(whichchild());
					parent->rot(whichchild());
				}
			}
		}
	}
};

struct linkcut
{
	vector<node> vn;
	linkcut(int n) : vn(n) {}
	void access(node* v) //creates a preferred path from the root to v and splays v
	{
		v->splay();
		if (v->child[1]) //remove preferred edge below
		{
			v->child[1]->parent = nullptr;
			v->child[1]->pathparent = v;
			v->child[1] = nullptr;
		}
		while (v->pathparent) //walk up the tree to create preferred path
		{
			v->pathparent->splay(); //make v the preferred child
			if (v->pathparent->child[1])
			{
				v->pathparent->child[1]->parent = nullptr;
				v->pathparent->child[1]->pathparent = v->pathparent;
			}
			v->pathparent->child[1] = v;
			v->parent = v->pathparent;
			v->pathparent = v->pathparent->pathparent; //rotate so that v is root
			v->parent->rot(true);
		}
	}
	void access(int v)
	{
		access(&vn[v]);
	}
	node* findroot(node* v) //finds the root of the tree v is in and splays it
	{
		access(v); //create preferred path to v
		while (v->child[0]) //walk to left to find root
		{
			v = v->child[0];
			v->proprev();
		}
		v->splay(); //splay to make future access fast
		return v;
	}
	node* findroot(int v)
	{
		return findroot(&vn[v]);
	}
	void cut(node* v) //disconnect v from its parent
	{
		access(v);
		assert(v->child[0]); //make sure we don't try to cut root
#ifdef _MSC_VER
#pragma warning(suppress: 6011)
#endif // _MSC_VER
		v->child[0]->pathparent = nullptr;
		v->child[0]->parent = nullptr;
		v->child[0] = nullptr;
	}
	void cut(int v)
	{
		cut(&vn[v]);
	}
	void link(node* v, node* w) //link v to w, v must be root
	{
		access(v);
		assert(!v->child[0]); //make sure v is root
		access(w);
		v->child[0] = w;
		w->parent = v;
		v->pathparent = w->pathparent;
	}
	void link(int v, int w)
	{
		link(&vn[v], &vn[w]);
	}
	void reroot(node* v) //make v the root
	{
		access(v);
		if (v->child[0])
		{
			v->child[0]->rev = !v->child[0]->rev;
			v->child[0]->pathparent = v;
			v->child[0]->parent = nullptr;
			v->child[0] = nullptr;
		}
	}
	void reroot(int v)
	{
		reroot(&vn[v]);
	}
};
