#include <string>

using namespace std;

const int maxn = 1005;

//KMP algorithm

//Based on pseudocode from https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm

string t, p;
int tn, pn, nt[maxn];

void buildtable()
{
	nt[0] = -1;
	int pos = 1, cnd = 0;
	for (; pos < pn; pos++, cnd++)
	{
		if (p[pos] == p[cnd])
		{
			nt[pos] = nt[cnd];
		}
		else
		{
			nt[pos] = cnd;
			do
			{
				cnd = nt[cnd];
			} while (cnd >= 0 && p[pos] != p[cnd]);
		}
	}
	nt[pos] = cnd;
}

int search()
{
	for (int j = 0, k = 0; j < tn;)
	{
		if (p[k] == t[j])
		{
			j++;
			k++;
			if (k == pn)
			{
				return j - k;
				//k = nt[k]; //for finding all occurrences
			}
		}
		else
		{
			k = nt[k];
			if (k < 0)
			{
				j++;
				k++;
			}
		}
	}
	return -1;
}
