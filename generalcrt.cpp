#include <iostream>
#include <cassert>
#include <utility>
using namespace std;

//Generalized Chinese Remainder Theorem For Non-Coprime Moduli Template

//CRT algorithm from CLRS and http://www.apfloat.org/crt.html
//Generalized algorithm from https://forthright48.com/chinese-remainder-theorem-part-2-non-coprime-moduli/
//Extended Euclidean Algorithm from https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Pseudocode
//Referenced implementation from https://github.com/kth-competitive-programming/kactl/blob/master/content/number-theory/CRT.h

//solution for x = a (mod p) and x = b (mod q) only exists if a = b (mod g), where g = gcd(p, q)
//solution is x = (a * v * q + b * u * p) / g (mod lcm(p, q)), where u and v are Bezout coefficients
//such that g = u * p + v * q

int n;

int exteuclid(int r, int nr, int& p, int& q) //g = p * a + q * b
{
	int s = 1, ns = 0, t = 0, nt = 1;
	while (nr)
	{
		int q = r / nr;
		int temp = s - q * ns;
		s = ns;
		ns = temp;
		temp = t - q * nt;
		t = nt;
		nt = temp;
		temp = r - q * nr;
		r = nr;
		nr = temp;
	}
	p = s;
	q = t;
	return r;
}

pair<int, int> generalcrt(int* a, int* m)
{
	int ta = 0, tm = 1;
	for (int i = 0; i < n; i++)
	{
		int p, q;
		int g = exteuclid(tm, m[i], p, q);
		assert((ta - a[i]) % g == 0);
		int nm = tm / g * m[i];
		ta = ((ta * (m[i] / g) % nm * q % nm + a[i] * (tm / g) % nm * p % nm) % nm + nm) % nm;
		tm = nm;
	}
	return { ta, tm };
}
