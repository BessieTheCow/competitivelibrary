#include <iostream>
using namespace std;

//Chinese Remainder Theorem

//Algorithm from CLRS and http://www.apfloat.org/crt.html

int n;

int inverse(int nr, int mod)
{
	int t = 0, nt = 1, r = mod;
	while (nr)
	{
		int q = r / nr;
		int temp = t - q * nt;
		t = nt;
		nt = temp;
		temp = r - q * nr;
		r = nr;
		nr = temp;
	}
	return (t + mod) % mod;
}

int crt(int* a, int* p)
{
	int r = 0, P = 1;
	for (int i = 0; i < n; i++)
	{
		P *= p[i];
	}
	for (int i = 0; i < n; i++)
	{
		int m = P / p[i];
		r = (r + a[i] * m % P * inverse(m, p[i]) % P) % P;
	}
	return r;
}
