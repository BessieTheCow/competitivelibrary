#include <iostream>
#include <vector>
using namespace std;

constexpr int maxn = 100005;

//Linear sieve for finding primes and computing multiplicative functions
//Based on https://codeforces.com/blog/entry/54090

int ipow(int b, int a)
{
	int r = 1;
	while (a)
	{
		if (a & 1)
		{
			r *= b;
		}
		b *= b;
		a >>= 1;
	}
	return r;
}

int n, cnt[maxn], f[maxn];
bool composite[maxn];
vector<int> primes;

int fp(int p) //computes f(p) for prime p
{
	return p - 1; //Euler's totient function
}

int fpk(int p, int k) //comptues f(p^k) for prime p
{
	return ipow(p, k) - ipow(p, k - 1); //Euler's totient function
}

int fpi(int p, int i) //computes f(pi) where p is the smallest prime divisor of i
{
	int t = ipow(p, cnt[i]);
	return i == t ? fpk(p, cnt[i] + 1) : f[i / t] * f[t * p];
}

void linearsieve()
{
	f[1] = 1;
	for (int i = 2; i <= n; i++)
	{
		if (!composite[i])
		{
			primes.push_back(i);
			f[i] = fp(i);
			cnt[i] = 1;
		}
		for (auto it = primes.begin(); it != primes.end() && *it * i <= n; ++it)
		{
			int p = *it;
			int product = p * i;
			composite[product] = true;
			if (i % p != 0)
			{
				f[product] = f[p] * f[i];
				cnt[product] = 1;
			}
			else
			{
				f[product] = fpi(p, i);
				cnt[product] = cnt[i] + 1;
				break;
			}
		}
	}
}
