// 426518

#ifndef LOWBITS_HPP
#define LOWBITS_HPP

#include <cassert>
#include <functional>
#include <iterator>
#include <vector>

#if LOWBITS_NOISE >= 1
#	define LOWBITS_NOISE1(x) x
#else
#	define LOWBITS_NOISE1(x)
#endif

#if LOWBITS_NOISE >= 2
#	define LOWBITS_NOISE2(x) x
#else
#	define LOWBITS_NOISE2(x)
#endif

#if LOWBITS_NOISE
#	include <iostream>
#endif

// FIXME: Step 1 doesn't work on initial run. (This is probably OK now.)

/* Potential optimizations:
   
   Remove mark area - probably can't do this.
   Optimize ascension so that there's a best match. */

/* Note: To do non power-of-two sorts, mask off the ends of the bit levels according to the bits of -input_size. */

class _lowbits_base
{
public:
	typedef typename std::vector<bool>::size_type size_type;

protected:
	std::vector<bool> _bit_bucket, _mark_area;
	std::vector<size_type> _bucket_levels;

	size_type _descend(std::vector<size_type>::const_iterator x, size_type n) const;
	_lowbits_base(size_type input_size);
};

template<typename RndIt, typename Pred = std::less<> > class lowbits: public _lowbits_base
{
private:
	RndIt _first;
	Pred _pr;

public:
	lowbits(RndIt first, size_type input_size, Pred pr = Pred());

	// template<typename RndIt, typename OutIt, typename Pred> OutIt lowbits_sort(RndIt first, RndIt last, OutIt x, Pred pr)
	size_type operator ()();
};

template<typename RndIt, typename Pred> lowbits<RndIt, Pred>::lowbits(RndIt first, size_type input_size, Pred pr):
	_lowbits_base(input_size), _first(first), _pr(pr)
{
	LOWBITS_NOISE1(std::cout << "Sort hath begun." << std::endl);

	// Step 0.1: Populate the trei; gather stating bit locations.

	size_type bucket_size = 0, level_size = input_size;

	while(level_size != 1)
	{
		level_size = (level_size + 1) >> 1;
		size_type level_first = bucket_size;

		bucket_size += level_size;

		for(size_type level_pt = 0; level_pt < level_size; ++level_pt)
		{
			_bit_bucket[level_first + level_pt] = !_pr(
				_first[_descend(_bucket_levels.end(), level_pt << 1)],
				_first[_descend(_bucket_levels.end(), (level_pt << 1) | 1)]);
		}

		_bucket_levels.push_back(level_first);
	}
}

template<typename RndIt, typename Pred> _lowbits_base::size_type lowbits<RndIt, Pred>::operator ()()
{
	// Termination is a wild guess.

	// Step 1: Descend to location of lowest bit.

	// Step 1.1: Reset level pointers because I don't feel like tweaking the ascender...yet.
	typename std::vector<size_type>::iterator level_it = _bucket_levels.end();
	size_type level_pt = 0;

	level_pt = _descend(level_it, level_pt);

	// Step 2: Output at location of bit.
	size_type result = level_pt;

	level_it = _bucket_levels.begin();
	level_pt >>= 1;

	// Step 3.0: Ascend until marked area is not empty.

	while(level_it != _bucket_levels.end() && _mark_area[*level_it + level_pt])
	{
		LOWBITS_NOISE2(std::cout << "x");

		++level_it;
		level_pt >>= 1;
	}

	if(level_it == _bucket_levels.end()) // Termination goeth here (?)
		return result;

	// Step 3.1: Flip a bit in the marked area, and ascend out.

	LOWBITS_NOISE2(std::cout << ";");

	_mark_area[*level_it + level_pt].flip();
	_bit_bucket[*level_it + level_pt].flip();

	++level_it;
	level_pt >>= 1;

	// Step 3.2: Flip bits & ascend until predicate is true.

	while(level_it != _bucket_levels.end())
	{
		size_type pt0 = _descend(level_it, (level_pt << 1));
		size_type pt1 = _descend(level_it, (level_pt << 1) | 1);

		LOWBITS_NOISE2(std::cout << pt0 << "," << pt1 << ":");

		if(!_mark_area[*level_it + level_pt] &&
			_bit_bucket[*level_it + level_pt] != !_pr(
				_first[pt0],
				_first[pt1]))
		{
			LOWBITS_NOISE2(std::cout << "1;");
			_bit_bucket[*level_it + level_pt].flip();
		}
		else
		{
			LOWBITS_NOISE2(std::cout << "0;");
		}

		++level_it;
		level_pt >>= 1;
	}

	LOWBITS_NOISE2(std::cout << std::endl);

	// Step 4: NEXT!
	return result;
}

namespace
{
	template<typename RndIt, typename OutIt, typename Pred> OutIt lowbits_sort(RndIt first, RndIt last, OutIt x, Pred pr)
	{
		typename std::iterator_traits<RndIt>::difference_type size = last - first;
		lowbits<RndIt, Pred> order(first, size, pr);

		LOWBITS_NOISE1(std::cout << "Step 1-4" << std::endl);
		while(size)
		{
			*x = first[order()];
			++x;
			--size;
		}
		LOWBITS_NOISE1(std::cout << "Sort hath ended." << std::endl);

		return x;
	}

	template<typename RndIt, typename OutIt> OutIt lowbits_sort(RndIt first, RndIt last, OutIt x)
	{
		return lowbits_sort(first, last, x, std::less<typename std::iterator_traits<RndIt>::value_type>());
	}
}

#endif
