#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

#ifdef LOCAL
struct timer
{
	clock_t starttime = clock();
	~timer()
	{
		clock_t endtime = clock();
#pragma warning(suppress: 26451)
		cout << fixed << setprecision(3) << static_cast<double>(endtime - starttime) / CLOCKS_PER_SEC << '\n';
	}
} tm;
#endif
