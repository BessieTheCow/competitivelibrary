#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

//Longest palindromic substring
//Complexity: O(n)

constexpr int maxn = 3005;

int p[2 * maxn + 1];

int lps(string s)
{
	string s1 = "|";
	for (char c : s)
	{
		s1 += c;
		s1 += '|';
	}
	int n = s1.size();
	p[0] = 0;
	int c = 0, l = 0, r = 0;
	for (int i = 1; i < n; i++)
	{
		if (i > r)
		{
			c = l = r = i;
			while (l > 0 && s1[l - 1] == s1[r + 1])
			{
				l--;
				r++;
			}
			p[i] = r - i;
		}
		else
		{
			int i1 = c - (i - c);
			if (i1 - p[i1] > l)
			{
				p[i] = p[i1];
			}
			else if (i1 - p[i1] == l)
			{
				c = i;
				l = i - (r - i);
				while (l > 0 && s1[l - 1] == s1[r + 1])
				{
					l--;
					r++;
				}
				p[i] = r - i;
			}
			else
			{
				p[i] = r - i;
			}
		}
	}
	int m = -1;
	for (int i = 0; i < n; i++)
	{
		m = max(m, p[i]);
	}
	return m;
}
