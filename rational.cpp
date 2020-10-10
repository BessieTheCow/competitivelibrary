#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;

//Rational number template
//Fraction simplification algorithm from: https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Simplification_of_fractions

typedef int inttype;

struct rational
{
	inttype num, deno;
	void simplify()
	{
		assert(deno != 0);
		inttype s = 0, olds = 1, t = 1, oldt = 0, r = deno, oldr = num;
		while (r)
		{
			inttype q = oldr / r - (oldr < 0);
			inttype nr = oldr - q * r;
			oldr = r;
			r = nr;
			inttype ns = olds - q * s;
			olds = s;
			s = ns;
			inttype nt = oldt - q * t;
			oldt = t;
			t = nt;
		}
		if (s < 0)
		{
			s = -s;
			t = -t;
		}
		num = -t;
		deno = s;
	}
	rational(inttype x = 0) : num(x), deno(1) {}
	rational(inttype n, inttype d) : num(n), deno(d) { simplify(); }
	explicit operator double() const
	{
		return static_cast<double>(num) / deno;
	}
	rational& operator+=(const rational&);
	rational& operator-=(const rational&);
	rational& operator*=(const rational&);
	rational& operator/=(const rational&);
};

rational operator+(const rational& a)
{
	return a;
}
rational operator-(const rational& a)
{
	rational r = a;
	r.num = -r.num;
	return r;
}
rational recip(rational a)
{
	assert(a.num != 0);
	swap(a.num, a.deno);
	if (a.deno < 0)
	{
		a.num = -a.num;
		a.deno = -a.deno;
	}
	return a;
}

rational& rational::operator+=(const rational& other)
{
	num = num * other.deno + deno * other.num;
	deno *= other.deno;
	simplify();
	return *this;
}
rational& rational::operator-=(const rational& other)
{
	return *this += -other;
}
rational& rational::operator*=(const rational& other)
{
	num *= other.num;
	deno *= other.deno;
	simplify();
	return *this;
}
rational& rational::operator/=(const rational& other)
{
	return *this *= recip(other);
}

rational operator+(rational a, const rational& b)
{
	a += b;
	return a;
}
rational operator-(rational a, const rational& b)
{
	a -= b;
	return a;
}
rational operator*(rational a, const rational& b)
{
	a *= b;
	return a;
}
rational operator/(rational a, const rational& b)
{
	a /= b;
	return a;
}

bool operator==(const rational& a, const rational& b)
{
	return a.num == b.num && a.deno == b.deno;
}
bool operator!=(const rational& a, const rational& b)
{
	return !(a == b);
}
bool operator<(const rational& a, const rational& b)
{
	return a.num * b.deno < a.deno * b.num;
}
bool operator>(const rational& a, const rational& b)
{
	return b < a;
}
bool operator<=(const rational& a, const rational& b)
{
	return !(b < a);
}
bool operator>=(const rational& a, const rational& b)
{
	return !(a < b);
}

ostream& operator<<(ostream& os, rational a)
{
	return os << a.num << '/' << a.deno;
}
