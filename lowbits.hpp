// 426518

#ifndef LOWBITS_HPP
#define LOWBITS_HPP

#include <cassert>
#include <functional>
#include <limits>
#include <memory>
#include <vector>

#if LOWBITS_DEBUG >= 1
#	define LOWBITS_DEBUG1(x) { std::cout << "* " << (x) << std::endl; }
#else
#	define LOWBITS_DEBUG1(x)
#endif

#if LOWBITS_DEBUG >= 2
#	define LOWBITS_DEBUG2(x) (std::cout << x << std::flush)
#else
#	define LOWBITS_DEBUG2(x)
#endif

#if LOWBITS_DEBUG
#	include <iostream>
#endif

// FIXME: Step 1 doesn't work on initial run. (This is probably OK now.)

class _lowbits_base
{
public:
	typedef typename std::vector<bool>::size_type size_type;

protected:
	typedef std::size_t _word_type;
	std::unique_ptr<_word_type []> _tree;
	std::vector<_word_type *> _bucket_levels; // TODO: This could be another unique_ptr instead of a vector.
	size_type _pt;

	enum
	{
		_word_bits = std::numeric_limits<_word_type>::digits
	};

	static size_type _word_offset(size_type offset_bits)
	{
		return offset_bits / _word_bits;
	}

	static size_type _word_mask(size_type offset_bits)
	{
		return _word_type(1) << (offset_bits & (_word_bits - 1));
	}

	static size_type _round2(size_type n)
	{
		return n & ~size_type(1);
	}

	size_type _descend(std::vector<_word_type *>::const_iterator x, size_type n) const;
	_lowbits_base(size_type input_size);
};

template<typename RndIt, typename Pred = std::less<typename std::iterator_traits<RndIt>::value_type> > class lowbits:
	public _lowbits_base
{
private:
	RndIt _first;
	Pred _pr;

public:
	lowbits(RndIt first, size_type input_size, Pred pr = Pred());

	// template<typename RndIt, typename OutIt, typename Pred> OutIt lowbits_sort(RndIt first, RndIt last, OutIt x, Pred pr)
	lowbits &operator ++();

	size_type operator *() const
	{
		// Step 2: Output at location of bit.
		return _pt;
	}
};

template<typename RndIt, typename Pred> lowbits<RndIt, Pred>::lowbits(RndIt first, size_type input_size, Pred pr):
	_lowbits_base(input_size), _first(first), _pr(pr)
{
	LOWBITS_DEBUG1(__FUNCTION__);

	// Step 0.1: Populate the trei; gather stating bit locations.

	size_type level_size = input_size;

	for(std::vector<_word_type *>::const_iterator level_it = _bucket_levels.begin(); level_it != _bucket_levels.end(); ++level_it)
	{
		_word_type *level_first = *level_it;
		size_type level_size1 = _round2(level_size);

		for(size_type level_pt = 0; level_pt != level_size1; level_pt += 2)
		{
			if(!_pr(
				_first[_descend(level_it, level_pt)],
				_first[_descend(level_it, level_pt | 1)]))
			{
				level_first[_word_offset(level_pt)] |= _word_mask(level_pt);
			}
		}

		if(level_size & 1)
			level_first[_word_offset(level_size1)] |= _word_mask(level_size1) << 1;

		level_size = (level_size + 1) >> 1;
	}

	// Step 1: Descend to location of lowest bit.
	_pt = _descend(_bucket_levels.end(), 0);
}

template<typename RndIt, typename Pred> lowbits<RndIt, Pred> &lowbits<RndIt, Pred>::operator ++()
{
	typename std::vector<_word_type *>::iterator level_it = _bucket_levels.begin();
	size_type level_pt = _pt;

	// Step 3.0: Ascend until marked area is not empty.
	for(;;)
	{
		if(level_it == _bucket_levels.end()) // Termination goeth here (?)
			return *this;

		size_type level_pt1 = _round2(level_pt);
		_word_type &level_word = (*level_it)[_word_offset(level_pt1)];
		_word_type level_mask = _word_mask(level_pt1);

		if(!(level_word & (level_mask << 1)))
		{
			// Step 3.1: Flip a bit in the marked area, and ascend out.
			LOWBITS_DEBUG2(";");
			level_word ^= level_mask | (level_mask << 1);

			// Step 3.2: Flip bits & ascend until predicate is true.
			_pt = _descend(level_it, level_pt1 | ((level_word & level_mask) != 0));
			break;
		}

		LOWBITS_DEBUG2("x");

		++level_it;
		level_pt >>= 1;
	}

	for(;;)
	{
		level_pt >>= 1;
		++level_it;

		if(level_it == _bucket_levels.end())
			break;

		size_type level_pt1 = _round2(level_pt);
		_word_type &level_word = (*level_it)[_word_offset(level_pt1)];
		_word_type level_mask = _word_mask(level_pt1);

		size_type pt0, pt1;
		if(level_word & level_mask)
		{
			pt0 = _descend(level_it, level_pt1);
			pt1 = _pt;
		}
		else
		{
			pt0 = _pt;
			pt1 = _descend(level_it, level_pt1 | 1);
		}

		assert(pt0 == _descend(level_it, level_pt1));
		assert(pt1 == _descend(level_it, level_pt1 | 1));

		LOWBITS_DEBUG2(pt0 << "," << pt1 << ":");

		if(!(level_word & (level_mask << 1)) && !(level_word & level_mask) != _pr(_first[pt0], _first[pt1]))
		{
			LOWBITS_DEBUG2("1;");
			level_word ^= level_mask;
		}
		else
		{
			LOWBITS_DEBUG2("0;");
		}

		_pt = (level_word & level_mask) ? pt1 : pt0;
	}

	LOWBITS_DEBUG2('\n');

	// Step 4: NEXT!
	return *this;
}

#endif
