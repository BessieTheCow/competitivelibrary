#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cassert>

using namespace std;

//Complexity: O(N log N)

constexpr int maxn = 200010;

//Suffix array

//Based on the implementation from
//https://github.com/stevenhalim/cpbook-code/blob/master/ch6/sa_lcp.cpp

int n; //length of string
string s; //the string
int r1[maxn], r2[maxn]; //rank array
int* ra = r1, * tempra = r2;
int sa1[maxn], sa2[maxn]; //suffix array
int* sa = sa1, * tempsa = sa2;
int c[max(maxn, 256)]; //count for counting sort
int phi[maxn]; //maps a suffix to the one before it in the suffix array
int plcp[maxn]; //plcp[i] = lcp of suffix starting at i and previous suffix in suffix array
int lcp[maxn]; //lcp[i] = lcp of i and i - 1 in suffix array

/*
sorts the suffixes based on the rank of their suffix starting from the kth character
*/
void countingsort(int k, int mr)
{
	fill_n(c, mr + 1, 0);
	for (int i = k; i < n; i++) //count the ranks
	{
		c[ra[i]]++;
	}
	int sum = k;
	for (int i = 0; i <= mr; i++) //add up the counts
	{
		int t = c[i];
		c[i] = sum;
		sum += t;
	}
	int ss = 0;
	for (int i = 0; i < n; i++) //move elements to new positions
	{
		tempsa[sa[i] + k < n ? c[ra[sa[i] + k]]++ : ss++] = sa[i];
	}
	swap(sa, tempsa);
}

void constructsa()
{
	n = static_cast<int>(s.size());
	for (int i = 0; i < n; i++) //initial rank = encoding value
	{
		ra[i] = static_cast<unsigned char>(s[i]);
	}
	for (int i = 0; i < n; i++) //initial suffix array
	{
		sa[i] = i;
	}
	int maxrank = 256;
	for (int k = 1; true; k *= 2) //loop over log2(n) steps
	{
		countingsort(k, maxrank); //sort by the rank of the second part
		countingsort(0, maxrank); //resort by the rank of the first part
		tempra[sa[0]] = 0;
		int r = 0; //current rank
		for (int i = 1; i < n; i++)
		{
			//assign new ranks
			tempra[sa[i]] = ra[sa[i]] == ra[sa[i - 1]] && sa[i - 1] + k < n && ra[sa[i] + k] == ra[sa[i - 1] + k] ? r : ++r;
		}
		swap(ra, tempra);
		maxrank = ra[sa[n - 1]];
		if (maxrank == n - 1)
		{
			break; //breaks if all ranks are unique
		}
	}
}

void computelcp()
{
	phi[sa[0]] = -1;
	for (int i = 1; i < n; i++)
	{
		phi[sa[i]] = sa[i - 1]; //fill in values of phi
	}
	for (int i = 0, l = 0; i < n; i++)
	{
		if (phi[i] == -1)
		{
			plcp[i] = 0;
			continue;
		}
		while (s[i + l] == s[phi[i] + l]) //relies on character past the end of a string to be '\0', guaranteed since C++11
		{
			l++; //find first different character of suffixes
		}
		plcp[i] = l;
		l = max(l - 1, 0); //can start at l - 1 for next suffix
	}
	for (int i = 0; i < n; i++)
	{
		assert(sa[i] >= 0 && sa[i] < n); //suppress warning about sa[i] being out of bounds
		lcp[i] = plcp[sa[i]]; //copy values to lcp array indexed by position in suffic array
	}
}
