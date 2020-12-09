#include <algorithm>
using namespace std;

//Hilbert curve for use as an alternative sorting order
//in Mo's algorithm

//Complexity: n * sqrt(q)

//Based on https://codeforces.com/blog/entry/61203
//and https://en.wikipedia.org/wiki/Hilbert_curve#Applications_and_mapping_algorithms
//(I posted this comment: https://codeforces.com/blog/entry/61203?#comment-522213)

constexpr int logmaxn = 20;
constexpr int maxn = 1 << logmaxn;

long long hilbertorder(int x, int y)
{
	long long d = 0;
	for (int s = 1 << (logmaxn - 1); s; s >>= 1)
	{
		bool rx = x & s, ry = y & s;
		d = (d << 2) | ((rx * 3) ^ static_cast<int>(ry));
		if (!ry)
		{
			if (rx)
			{
				x = maxn - x;
				y = maxn - y;
			}
			swap(x, y);
		}
	}
	return d;
}
