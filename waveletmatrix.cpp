#include <algorithm>
using namespace std;

//Complexity: Logarithmic in size of numbers in array

//Wavelet matrix
//Based on https://users.dcc.uchile.cl/~jperez/papers/ioiconf16.pdf
//and https://users.dcc.uchile.cl/~gnavarro/ps/is14.pdf

//0-indexed, inclusive bounds

constexpr int maxn = 100005, logs = 30;

int n;
int a[maxn], cl[logs][maxn + 1], z[logs];
int p[maxn];

void build()
{
	copy_n(a, n, p);
	for (int i = 0, mask = 1 << (logs - 1); i < logs; i++, mask >>= 1)
	{
		for (int j = 1; j <= n; j++)
		{
			cl[i][j] = cl[i][j - 1] + ((p[j - 1] & mask) != 0);
		}
		z[i] = n - cl[i][n];
		stable_partition(p, p + n, [mask](int x) { return !(x & mask); });
	}
}

int mapleft(int d, int i)
{
	return i - cl[d][i + 1];
}

int mapright(int d, int i)
{
	return z[d] + cl[d][i + 1] - 1;
}

int cnt(int q, int l, int r)
{
	l--;
	for (int d = 0, mask = 1 << (logs - 1); d < logs; d++, mask >>= 1)
	{
		if (q & mask)
		{
			l = mapright(d, l);
			r = mapright(d, r);
		}
		else
		{
			l = mapleft(d, l);
			r = mapleft(d, r);
		}
	}
	return r - l;
}

int cnt(int q, int i)
{
	return cnt(q, 0, i);
}

int quantile(int k, int l, int r)
{
	l--;
	int result = 0;
	for (int i = 0, mask = 1 << (logs - 1); i < logs; i++, mask >>= 1)
	{
		int ll = mapleft(i, l), lr = mapleft(i, r);
		int c = lr - ll;
		if (k <= c)
		{
			l = ll;
			r = lr;
		}
		else
		{
			result |= mask;
			l = mapright(i, l);
			r = mapright(i, r);
			k -= c;
		}
	}
	return result;
}

int cnt(int x, int y, int l, int r, int d, int lo)
{
	static_assert(logs < 31, "1 << logs will overflow");
	int hi = lo + (1 << (logs - d));
	if (x >= hi || y < lo)
	{
		return 0;
	}
	if (x <= lo && y >= hi - 1)
	{
		return r - l;
	}
	return cnt(x, y, mapleft(d, l), mapleft(d, r), d + 1, lo)
		+ cnt(x, y, mapright(d, l), mapright(d, r), d + 1, lo + (1 << (logs - d - 1)));
}

int cnt(int x, int y, int l, int r)
{
	return cnt(x, y, l - 1, r, 0, 0);
}

void swp(int oi)
{
	if (a[oi] == a[oi + 1])
	{
		return;
	}
	int i = oi;
	int d = 0, mask = 1 << (logs - 1);
	for (; (a[oi] & mask) == (a[oi + 1] & mask); d++, mask >>= 1)
	{
		if (a[oi] & mask)
		{
			i = mapright(d, i);
		}
		else
		{
			i = mapleft(d, i);
		}
	}
	cl[d][i + 1] += a[oi] & mask ? -1 : 1;
	swap(a[oi], a[oi + 1]);
}
