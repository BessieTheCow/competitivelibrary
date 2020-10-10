#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <climits>
using namespace std;

//Representing numbers by vectors mod 2 to solve subset XOR problems
//Based on https://drschwad.github.io/2019-08-06-z2-space-xor-trick/

constexpr int logs = 20;

int fsb(int x)
{
#ifdef _MSC_VER
	unsigned long index;
	_BitScanReverse(&index, x);
	return index;
#else
	return sizeof(int) * CHAR_BIT - 1 - __builtin_clz(x);
#endif
}

int basis[logs], sz = 0; //basis[i] contains a vector in the basis which has i as the first set bit

void insert(int x)
{
	if (sz == logs)
	{
		return;
	}
	while (x)
	{
		int i = fsb(x);
		if (basis[i] == 0)
		{
			basis[i] = x;
			sz++;
			break;
		}
		else
		{
			x ^= basis[i];
		}
	}
}

bool representable(int x)
{
	if (sz == logs)
	{
		return true;
	}
	while (x)
	{
		int i = fsb(x);
		if (basis[i] == 0)
		{
			return false;
		}
		else
		{
			x ^= basis[i];
		}
	}
	return true;
}
