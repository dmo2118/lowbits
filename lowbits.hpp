#ifndef LOWBITS_HPP
#define LOWBITS_HPP

#include "lowbits1.hpp"

#define PROCESSOR_MHZ ((unsigned __int64)450000000)

#if defined(_MSC_VER)

#pragma warning(push, 1)

static __inline unsigned __int64 rdtsc()
{
	__asm _emit 0x0F __asm _emit 0x31; // rdtsc
}

#pragma warning(pop)

#elif defined(__GNUC__)

// I'm broken.
/*
static __inline __fastcall unsigned long long rdtsc()
{
	asm("rdtsc");

	return 0;
}
*/

/*
#include <ctime>

static __inline __fastcall unsigned long long rdtsc()
{
	return (unsigned long long)std::clock() * PROCESSOR_MHZ / CLOCKS_PER_SEC;
}
*/

#include <windows.h>
static __inline __fastcall unsigned long long rdtsc()
{
	LARGE_INTEGER t, frq;

	QueryPerformanceCounter(&t);
	QueryPerformanceFrequency(&frq);

	return t.QuadPart * PROCESSOR_MHZ / frq.QuadPart;
}

#else
#error Fixme, todo, et cetera.
#endif

template<class T> class counted_less
{
private:
	size_t &_ops;
public:
	counted_less(size_t &ops) : _ops(ops)
	{
	}

	bool operator()(const T &x, const T &y)
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

typedef unsigned short type;

namespace std
{
	template<> struct iterator_traits<type *> : public iterator<random_access_iterator_tag, type>
	{
	};
}

#endif
