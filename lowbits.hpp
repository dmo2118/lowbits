// 426518

#ifndef LOWBITS_HPP
#define LOWBITS_HPP

// Note: Code here is suboptimal. This algorithm may never be fast, but at least it can be a bit faster.

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

	class _bit_ref
	{
	private:
		_word_type *_ptr;
		_word_type _mask;

	public:
		_bit_ref(_word_type *words, size_type offset_bits):
			_ptr(words + (offset_bits / _word_bits)),
			_mask(_word_type(1) << (offset_bits & (_word_bits - 1)))
		{
		}

		operator bool() const
		{
			return *_ptr & _mask;
		}

		const _bit_ref &operator =(bool x) const
		{
			if(x)
				*_ptr |= _mask;
			else
				*_ptr &= ~_mask;
			return *this;
		}

		void flip() const
		{
			*_ptr ^= _mask;
		}
	};

	_bit_ref _tree0(_word_type *base, size_t n, bool mark = false)
	{
		return _bit_ref(base, (n << 1) | mark);
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

		for(size_type level_pt = 0; level_pt < (level_size >> 1); ++level_pt)
		{
			_tree0(level_first, level_pt) = !_pr(
				_first[_descend(level_it, level_pt << 1)],
				_first[_descend(level_it, (level_pt << 1) | 1)]);
		}

		if(level_size & 1)
		{
			_tree0(level_first, (level_size >> 1)) = false;
			_tree0(level_first, (level_size >> 1), true) = true;
		}

		level_size = (level_size + 1) >> 1;
	}

	// Step 1: Descend to location of lowest bit.
	_pt = _descend(_bucket_levels.end(), 0);
}

template<typename RndIt, typename Pred> lowbits<RndIt, Pred> &lowbits<RndIt, Pred>::operator ++()
{
	typename std::vector<_word_type *>::iterator level_it = _bucket_levels.begin();
	size_type level_pt = _pt >> 1;

	// Step 3.0: Ascend until marked area is not empty.

	while(level_it != _bucket_levels.end() && _tree0(*level_it, level_pt, true))
	{
		LOWBITS_DEBUG2("x");

		++level_it;
		level_pt >>= 1;
	}

	if(level_it == _bucket_levels.end()) // Termination goeth here (?)
		return *this;

	// Step 3.1: Flip a bit in the marked area, and ascend out.

	LOWBITS_DEBUG2(";");

	_tree0(*level_it, level_pt, true).flip();
	_tree0(*level_it, level_pt).flip();

	// Step 3.2: Flip bits & ascend until predicate is true.

	_pt = _descend(level_it, (level_pt << 1) | _tree0(*level_it, level_pt));

	for(;;)
	{
		++level_it;
		level_pt >>= 1;

		if(level_it == _bucket_levels.end())
			break;

		size_type pt0, pt1;
		if(_tree0(*level_it, level_pt))
		{
			pt0 = _descend(level_it, (level_pt << 1));
			pt1 = _pt;
		}
		else
		{
			pt0 = _pt;
			pt1 = _descend(level_it, (level_pt << 1) | 1);
		}

		assert(pt0 == _descend(level_it, (level_pt << 1)));
		assert(pt1 == _descend(level_it, (level_pt << 1) | 1));

		LOWBITS_DEBUG2(pt0 << "," << pt1 << ":");

		if(!_tree0(*level_it, level_pt, true) && _tree0(*level_it, level_pt) != !_pr(_first[pt0], _first[pt1]))
		{
			LOWBITS_DEBUG2("1;");
			_tree0(*level_it, level_pt).flip();
		}
		else
		{
			LOWBITS_DEBUG2("0;");
		}

		_pt = _tree0(*level_it, level_pt) ? pt1 : pt0;
	}

	LOWBITS_DEBUG2('\n');

	// Step 4: NEXT!
	return *this;
}

#endif
