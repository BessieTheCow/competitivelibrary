#ifdef _MSC_VER
#include <intrin.h>
#else
#include <climits>
#endif
using namespace std;

//Finds index of first set bit and last set bit

int firstsetbit(int x)
{
#ifndef _MSC_VER
	return sizeof(int) * CHAR_BIT - 1 - __builtin_clz(x);
#else
	unsigned long index;
	_BitScanReverse(&index, x);
	return index;
#endif
}

int lastsetbit(int x)
{
#ifndef _MSC_VER
	return __builtin_ctz(x);
#else
	unsigned long index;
	_BitScanForward(&index, x);
	return index;
#endif
}
