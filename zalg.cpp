#include <string>

const int maxn = 1005;

//Z algorithm

//Based on https://codeforces.com/blog/entry/3107

int n, z[maxn];
string s;

void zalg()
{
	int l = 0, r = 0;
	for (int i = 1; i < n; i++)
	{
		if (i > r)
		{
			l = i;
			r = i;
			while (r < n && s[r - l] == s[r])
			{
				r++;
			}
			z[i] = r - l;
			r--;
		}
		else
		{
			int k = i - l;
			if (z[k] < r - i + 1)
			{
				z[i] = z[k];
			}
			else
			{
				l = i;
				while (r < n && s[r - l] == s[r])
				{
					r++;
				}
				z[i] = r - l;
				r--;
			}
		}
	}
}
