#include <cmath>
#include <complex>
#include <vector>
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <climits>
#endif
using namespace std;

//long double or direct root computation may be required, remember to add L to tau
//doubles with long double root computation are safe when
//(sum(a[i]^2) + sum(b[i]^2)) * log2(n) < 9e14 for regular convolution,
//n * log2(n) * mod < 8.6e14 for FFTMod, usually greater especially for random inputs
//to improve precision for FFT killer tests, randomly choose x and y coprime to mod,
//multiply one polynomial by x and the other by y, and multiply the result by (xy)^-1

//currently faster than NTTMod when using doubles

//Fast Fourier Transform
//Algorithm and optimizations from many sources,
//including but not limited to:
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
//https://github.com/simonlindholm/fft-precision/blob/master/fft-precision.md

//Add L to the end if using long double
constexpr double tau = 6.28318530717958647692528676655900576839433879875021;

constexpr int maxn = 1 << 20;

constexpr int mod = 1000000007, base = 31623;

//first half of 2^kth roots are stored in [2^(k - 1), 2^k)
complex<double> roots[maxn];

//computes roots by multiplying power of 2 roots
//uses O(log n) sin and cos operations and computes each root
//as the product of at most log n directly computed roots
void computeroots(int n)
{
	roots[1] = 1;
	for (int k = 2; k < n; k *= 2)
	{
#ifdef _MSC_VER
#pragma warning(suppress: 26451)
#endif
		double theta = tau / (k * 2);
		complex<double> w(cos(theta), sin(theta));
		for (int i = k / 2; i < k; i++)
		{
			roots[i * 2] = roots[i];
			roots[i * 2 + 1] = roots[i] * w;
		}
	}
}

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

//in-place decimation-in-frequency FFT without the bit-reversal permutation
//outputs in bit-reversed order
void diffft(complex<double>* a, int n)
{
	for (int k = n / 2; k >= 1; k /= 2)
	{
		for (int i = 0; i < n; i += k * 2)
		{
			for (int j = 0; j < k; j++)
			{
				complex<double> temp = roots[k + j] * (a[i + j] - a[i + j + k]);
				a[i + j] += a[i + j + k];
				a[i + j + k] = temp;
			}
		}
	}
}

//in-place decimation-in-time FFT without the bit-reversal permutation
//takes input in bit-reversed order
void ditfft(complex<double>* a, int n)
{
	for (int k = 1; k < n; k *= 2)
	{
		for (int i = 0; i < n; i += k * 2)
		{
			for (int j = 0; j < k; j++)
			{
				complex<double> z = roots[k + j] * a[i + j + k];
				a[i + j + k] = a[i + j] - z;
				a[i + j] += z;
			}
		}
	}
}

//multiplying integer polynomials using 2-in-1 trick
void multiply(const int* a, const int* b, long long* c, int n)
{
	if (n == 1)
	{
		c[0] = static_cast<long long>(a[0]) * b[0];
		return;
	}
	vector<complex<double>> h;
	h.reserve(n);
	for (int i = 0; i < n; i++)
	{
		h.emplace_back(a[i], b[i]);
	}
	diffft(h.data(), n);
	for (int i = 0; i < n; i++)
	{
		h[i] *= h[i];
	}
	h[0] = h[0] - conj(h[0]);
	h[1] = h[1] - conj(h[1]);
	for (int k = 2; k < n; k *= 2)
	{
		for (int i = k, j = k * 2 - 1; i < j; i++, j--)
		{
			complex<double> temp = h[i] - conj(h[j]);
			h[i] = h[j] - conj(h[i]);
			h[j] = temp;
		}
	}
	ditfft(h.data(), n);
	for (int i = 0; i < n; i++)
	{
#ifdef _MSC_VER
#pragma warning(suppress: 26451)
#endif
		c[i] = static_cast<long long>(round(h[i].imag() / (4 * n)));
	}
}

//multiplies integer polynomials mod an arbitrary 32-bit modulus
//uses 2-in-1 trick to reduce number of fft operations to 4
template<int m = mod>
void multiplymod(const int* a, const int* b, int* c, int n)
{
	if (n == 1)
	{
		c[0] = static_cast<long long>(a[0]) * b[0] % m;
		return;
	}
	vector<complex<double>> f, g;
	f.reserve(n);
	g.reserve(n);
	for (int i = 0; i < n; i++)
	{
		f.emplace_back(static_cast<double>(a[i] / base), static_cast<double>(a[i] % base));
		g.emplace_back(static_cast<double>(b[i] / base), static_cast<double>(b[i] % base));
	}
	diffft(f.data(), n);
	diffft(g.data(), n);
	complex<double> f0 = f[0];
	f[0] = (f[0] + conj(f[0])) * g[0];
	g[0] = (f0 - conj(f0)) * g[0];
	complex<double> f1 = f[1];
	f[1] = (f[1] + conj(f[1])) * g[1];
	g[1] = (f1 - conj(f1)) * g[1];
	for (int k = 2; k < n; k *= 2)
	{
		for (int i = k, j = k * 2 - 1; i < j; i++, j--)
		{
			complex<double> fi = f[i], gi = g[i];
			g[i] = (f[j] - conj(f[i])) * g[j];
			f[i] = (f[j] + conj(f[i])) * g[j];
			g[j] = (fi - conj(f[j])) * gi;
			f[j] = (fi + conj(f[j])) * gi;
		}
	}
	ditfft(f.data(), n);
	ditfft(g.data(), n);
	for (int i = 0; i < n; i++)
	{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26451)
#endif
		long long av = static_cast<long long>(round(f[i].real() / (2 * n)));
		long long cv = static_cast<long long>(round(g[i].real() / (-2 * n)));
		long long bv = static_cast<long long>(round(f[i].imag() / (2 * n)))
			+ static_cast<long long>(round(g[i].imag() / (2 * n)));
#ifdef _MSC_VER
#pragma warning(pop)
#endif
		c[i] = ((av % m * base + bv) % m * base + cv) % m;
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
#else
	unsigned long index;
	_BitScanReverse(&index, x);
	return index;
#endif
}

//Don't forget to compute roots and potentially bit-reversal order after setting n
