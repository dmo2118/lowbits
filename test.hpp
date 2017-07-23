#ifndef TEST_HPP
#define TEST_HPP

#ifdef _WIN32
#	define NOMINMAX
#endif

#include "lowbits.hpp"

#if 0 // defined __unix__

/* clock() works well enough on Linux. No need for gettimeofday(2). */

#include <sys/time.h>

namespace
{
	static inline unsigned long long clock_ull()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000000ull + tv.tv_usec;
	}

	static inline unsigned long clock_ull_frq()
	{
		return 1000000;
	}
}

#elif defined _WIN32

/* MSVCRT clock() has 1-2 ms resolution. Pretty mediocre. */

#include <windows.h>

namespace
{
	inline unsigned long long clock_ull()
	{
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		return t.QuadPart;
	}

	inline unsigned long long clock_ull_frq()
	{
		LARGE_INTEGER frq;
		QueryPerformanceFrequency(&frq);
		return frq.QuadPart;
	}
}

#else

#include <ctime>

namespace
{
	inline unsigned long long clock_ull()
	{
		return std::clock();
	}

	inline unsigned long long clock_ull_frq()
	{
		return CLOCKS_PER_SEC;
	}
}

#endif

class counted_less
{
private:
	size_t &_ops;
public:
	counted_less(size_t &ops): _ops(ops)
	{
	}

	template<typename T, typename U> bool operator ()(const T &x, const U &y) const
	{
		++_ops;
		return x < y;
	}
};

class null_object
{
public:
	null_object operator *() const
	{
		return *this;
	}

	null_object operator ++() const
	{
		return *this;
	}

	template<class T> null_object operator =(const T &) const
	{
		return *this;
	}
};

template<typename RndIt, typename OutIt, typename Pred> OutIt lowbits_sort(RndIt first, RndIt last, OutIt x, Pred pr)
{
	typename std::iterator_traits<RndIt>::difference_type size = last - first;
	lowbits<RndIt, Pred> order(first, size, pr);

	LOWBITS_DEBUG1(__FUNCTION__);
	while(size)
	{
		*x = first[order()];
		++x;
		--size;
	}
	LOWBITS_DEBUG1("lowbits_sort done");

	return x;
}

template<typename RndIt, typename OutIt> OutIt lowbits_sort(RndIt first, RndIt last, OutIt x)
{
	return lowbits_sort(first, last, x, std::less<typename std::iterator_traits<RndIt>::value_type>());
}

typedef unsigned short type;

#endif
