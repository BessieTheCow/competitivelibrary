#include <iterator>
using namespace std;

//Finds primitive root for NTT

constexpr int mod = 998244353;
constexpr int phi = mod - 1;
constexpr int primefactors[] = { 2, 7, 17 };
constexpr int npf = size(primefactors);

int modexp(int b, int e)
{
	int result = 1;
	while (e)
	{
		if (e & 1)
		{
			result = static_cast<long long>(result)* b% mod;
		}
		b = static_cast<long long>(b)* b% mod;
		e >>= 1;
	}
	return result;
}

int primitiveroot()
{
	for (int i = 2; i < mod; i++)
	{
		int j = 0;
		for (; j < npf; j++)
		{
			if (modexp(i, phi / primefactors[j]) == 1)
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
