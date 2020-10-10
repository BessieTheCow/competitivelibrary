#include <iostream>
using namespace std;

//Modular arithmetic

constexpr int mod = 1000000007;

int inverse(int nr)
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

struct modular
{
	int value;
	modular(int x = 0) : value(x% mod) {}
	explicit operator int() const { return value; }
	modular& operator+=(const modular&);
	modular& operator-=(const modular&);
	modular& operator*=(const modular&);
	modular& operator/=(const modular&);
};

modular operator+(const modular& x)
{
	return x;
}
modular operator-(const modular& x)
{
	return x.value > 0 ? mod - x.value : 0;
}
modular inverse(modular x)
{
	return inverse(x.value);
}

modular& modular::operator+=(const modular& other)
{
	value = (value + other.value) % mod;
	return *this;
}
modular& modular::operator-=(const modular& other)
{
	return *this += -other;
}
modular operator++(modular& x, int)
{
	modular t = x;
	x += 1;
	return t;
}
modular operator--(modular& x, int)
{
	modular t = x;
	x -= 1;
	return t;
}
modular& operator++(modular& x)
{
	return x += 1;
}
modular& operator--(modular& x)
{
	return x -= 1;
}
modular& modular::operator*=(const modular& other)
{
	value = (static_cast<long long>(value)* other.value) % mod;
	return *this;
}
modular& modular::operator/=(const modular& other)
{
	return *this *= inverse(other);
}

modular operator+(modular a, const modular& b)
{
	return a += b;
}
modular operator-(modular a, const modular& b)
{
	return a -= b;
}
modular operator*(modular a, const modular& b)
{
	return a *= b;
}
modular operator/(modular a, const modular& b)
{
	return a /= b;
}

modular modexp(modular b, int e)
{
	modular result = 1;
	while (e)
	{
		if (e & 1)
		{
			result *= b;
		}
		b *= b;
		e >>= 1;
	}
	return result;
}

ostream& operator<<(ostream& os, const modular& a)
{
	return os << a.value;
}
