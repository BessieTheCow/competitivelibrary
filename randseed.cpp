#include <cstdint>
#include <random>
#include <chrono>
#include <memory>
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
using namespace std;

//Non-deterministic random seed for Codeforces
//Based on https://codeforces.com/blog/entry/60442
//Hash function from https://codeforces.com/blog/entry/62393
//splitmix64 from http://xoshiro.di.unimi.it/splitmix64.c

auto highresclock = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
auto steadyclock = chrono::duration_cast<chrono::nanoseconds>(chrono::steady_clock::now().time_since_epoch()).count();
auto cyclecount = __rdtsc();

seed_seq seq
{
	static_cast<uint_least32_t>(random_device()()),
	static_cast<uint_least32_t>(highresclock),
	static_cast<uint_least32_t>(highresclock >> 32),
	static_cast<uint_least32_t>(steadyclock),
	static_cast<uint_least32_t>(steadyclock >> 32),
	static_cast<uint_least32_t>(cyclecount),
	static_cast<uint_least32_t>(cyclecount >> 32),
	reinterpret_cast<uint_least32_t>(make_unique<char>().get())
};

mt19937 g(seq);

struct hsh
{
	static uint_least64_t splitmix64(uint_least64_t x)
	{
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}
	size_t operator()(uint_least64_t x) const
	{
		static const uint_least64_t r = g();
		return splitmix64(x + r);
	}
};
