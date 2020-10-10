#include <vector>
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <climits>
#endif // _MSC_VER
using namespace std;

//May require C++17

//Number theoretic transform
//Algorithm and optimizations from many sources including:
//CLRS Chapter 30
//http://neerc.ifmo.ru/trains/toulouse/2017/fft.pdf
//http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
//https://codeforces.com/blog/entry/43499
//https://codeforces.com/blog/entry/48798
//https://cp-algorithms.com/algebra/fft.html
//https://en.wikipedia.org/wiki/Fast_Fourier_transform
//https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
//https://en.wikipedia.org/wiki/Butterfly_diagram
//https://rosettacode.org/wiki/Fast_Fourier_transform
//https://github.com/bqi343/USACO/blob/master/Implementations/content/numerical/Polynomials/FFT.h
//https://github.com/kth-competitive-programming/kactl/blob/master/content/numerical/FastFourierTransform.h

constexpr int modk = 23; //998244353
constexpr int modc = 119;
constexpr int mod = (modc << modk) + 1; //2^k * c + 1
constexpr int primefactors[] = { 2, 7, 17 }; //prime factors of mod - 1
constexpr int modk0 = 20, modk1 = 20, modk2 = 20;
constexpr int modc0 = 997, modc1 = 1003, modc2 = 1005;
constexpr int mod0 = (modc0 << modk0) + 1; //1045430273
constexpr int mod1 = (modc1 << modk1) + 1; //1051721729
constexpr int mod2 = (modc2 << modk2) + 1; //1053818881
constexpr int primefactors0[] = { 2, 997 };
constexpr int primefactors1[] = { 2, 17, 59 };
constexpr int primefactors2[] = { 2, 3, 5, 67 };
constexpr int moda = 1000000007;

template<int m = mod>
constexpr int sreduc(int x)
{
	return x < m ? x : x - m;
}

template<int m = mod>
constexpr int snreduc(int x)
{
	return x < 0 ? x + m : x;
}

template<int m = mod>
constexpr int inverse(int nr)
{
	int t = 0, nt = 1, r = m;
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
	return snreduc<m>(t);
}

template<int m = mod>
constexpr int modexp(int b, int e)
{
	int result = 1;
	while (e)
	{
		if (e & 1)
		{
			result = static_cast<long long>(result) * b % m;
		}
		b = static_cast<long long>(b) * b % m;
		e >>= 1;
	}
	return result;
}

template<int m = mod, int npf, const int(&pf)[npf] = primefactors>
constexpr int findprimitiveroot()
{
	for (int i = 2; i < m; i++)
	{
		int j = 0;
		for (; j < npf; j++)
		{
			if (modexp<m>(i, (m - 1) / pf[j]) == 1)
			{
				break;
			}
		}
		if (j == npf)
		{
			return i;
		}
	}
	return -1;
}



template<int mk = modk, int mc = modc,
	int pr = findprimitiveroot<(mc << mk) + 1, size(primefactors), primefactors>()>
	struct Roots
{
	int r[mk + 1];
	constexpr Roots() : r()
	{
		constexpr int m = (mc << mk) + 1;
		constexpr int fr = modexp<m>(pr, mc);
		r[mk] = fr;
		for (int i = mk - 1; i >= 0; i--)
		{
#ifdef _MSC_VER
#pragma warning(suppress: 26451)
#endif // _MSC_VER
			r[i] = static_cast<long long>(r[i + 1]) * r[i + 1] % m;
		}
	}
	constexpr int operator[](int i) const
	{
		return r[i];
	}
};

//bit-reversal permutation
vector<int> computerev(int logn)
{
	int n = 1 << logn;
	vector<int> rev;
	rev.reserve(n);
	rev.push_back(0);
	for (int i = 1; i < n; i++)
	{
		rev.push_back(rev[i >> 1] >> 1 | (i & 1) << (logn - 1));
	}
	return rev;
}

//in-place decimation-in-frequency NTT without the bit-reversal permutation
//outputs in bit-reversed order
template<int mk = modk, int mc = modc,
	int pr = findprimitiveroot<(mc << mk) + 1, size(primefactors), primefactors>()>
	void difntt(int* a, int logn)
{
	constexpr int m = (mc << mk) + 1;
	constexpr Roots<mk, mc, pr> rts;
	int n = 1 << logn;
	for (int k = n / 2, lk = logn; k >= 1; k /= 2, lk--)
	{
		for (int i = 0; i < n; i += k * 2)
		{
			int w = 1;
			for (int j = 0; j < k; j++)
			{
#ifdef _MSC_VER
#pragma warning(suppress: 26451)
#endif // _MSC_VER
				int temp = static_cast<long long>(w) * (a[i + j] - a[i + j + k] + m) % m;
				a[i + j] = sreduc<m>(a[i + j] + a[i + j + k]);
				a[i + j + k] = temp;
				w = static_cast<long long>(w) * rts[lk] % m;
			}
		}
	}
}

//in-place decimation-in-time inverse NTT without the bit-reversal permutation
//takes input in bit-reversed order
template<int mk = modk, int mc = modc,
	int pr = findprimitiveroot<(mc << mk) + 1, size(primefactors), primefactors>()>
	void invditntt(int* a, int logn)
{
	constexpr int m = (mc << mk) + 1;
	constexpr Roots<mk, mc, inverse<m>(pr)> irts;
	int n = 1 << logn;
	for (int k = 1, lk = 1; k < n; k *= 2, lk++)
	{
		for (int i = 0; i < n; i += k * 2)
		{
			int w = 1;
			for (int j = 0; j < k; j++)
			{
				int z = static_cast<long long>(w) * a[i + j + k] % m;
				a[i + j + k] = snreduc<m>(a[i + j] - z);
				a[i + j] = sreduc<m>(a[i + j] + z);
				w = static_cast<long long>(w) * irts[lk] % m;
			}
		}
	}
	int invn = inverse<m>(n);
	for (int i = 0; i < n; i++)
	{
		a[i] = static_cast<long long>(a[i]) * invn % m;
	}
}

//multiplies a with b and stores result in a, will modify b, remember to fill trailing zeros
template<int mk = modk, int mc = modc, int npf = size(primefactors), const int(&pf)[npf] = primefactors>
void multiply(int* a, int* b, int logn)
{
	constexpr int m = (mc << mk) + 1;
	constexpr int pr = findprimitiveroot<m, size(pf), pf>();
	int n = 1 << logn;
	difntt<mk, mc, pr>(a, logn);
	difntt<mk, mc, pr>(b, logn);
	for (int i = 0; i < n; i++)
	{
		a[i] = static_cast<long long>(a[i]) * b[i] % m;
	}
	invditntt<mk, mc, pr>(a, logn);
}

//multiplies a with b and stores result in a, will modify b, remember to fill trailing zeros
template<int mk0 = modk0, int mc0 = modc0,
	int npf0 = size(primefactors0), const int(&pf0)[npf0] = primefactors0,
	int mk1 = modk1, int mc1 = modc1,
	int npf1 = size(primefactors1), const int(&pf1)[npf1] = primefactors1,
	int mk2 = modk2, int mc2 = modc2,
	int npf2 = size(primefactors2), const int(&pf2)[npf2] = primefactors2,
	int ma = moda>
	void multiplymod(int* a, int* b, int logn)
{
	constexpr int m0 = (mc0 << mk0) + 1, m1 = (mc1 << mk1) + 1, m2 = (mc2 << mk2) + 1;
	constexpr int r01 = inverse<m1>(m0), r02 = inverse<m2>(m0), r12 = inverse<m2>(m1);
	int n = 1 << logn;
	vector<int> a1, a2, b1, b2;
	a1.reserve(n);
	a2.reserve(n);
	b1.reserve(n);
	b2.reserve(n);
	for (int i = 0; i < n; i++)
	{
		a1.push_back(a[i] % m1);
		a2.push_back(a[i] % m2);
		b1.push_back(b[i] % m1);
		b2.push_back(b[i] % m2);
		a[i] %= m0;
		b[i] %= m0;
	}
	multiply<mk0, mc0, size(pf0), pf0>(a, b, logn);
	multiply<mk1, mc1, size(pf1), pf1>(a1.data(), b1.data(), logn);
	multiply<mk2, mc2, size(pf2), pf2>(a2.data(), b2.data(), logn);
	for (int i = 0; i < n; i++)
	{
		int x0 = a[i];
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26451)
#endif // _MSC_VER
		int x1 = static_cast<long long>(a1[i] - x0 % m1 + m1) * r01 % m1;
		int x2 = (static_cast<long long>(a2[i] - x0 % m2 + m2) * r02 % m2 - x1 % m2 + m2) * r12 % m2;
#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER
		a[i] = (x0 % ma + static_cast<long long>(x1 % ma) * (m0 % ma) % ma
			+ static_cast<long long>(x2 % ma) * (m0 % ma) % ma * (m1 % ma) % ma) % ma;
	}
}

int firstsetbit(int x)
{
	if (!x)
	{
		return -1;
	}
#ifndef _MSC_VER
	return sizeof(int) * CHAR_BIT - 1 - __builtin_clz(x);
#else // _MSC_VER
	unsigned long index;
	_BitScanReverse(&index, x);
	return index;
#endif // !_MSC_VER
}

constexpr int logmaxn = 20;
static_assert(logmaxn <= modk);
constexpr int maxn = 1 << logmaxn;

//Don't forget to compute rev if needed
