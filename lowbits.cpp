#include "lowbits.hpp"

_lowbits_base::size_type _lowbits_base::_descend(std::vector<size_type>::const_iterator x, size_type n) const
{
	while(x != _bucket_levels.begin())
	{
		--x;
		n <<= 1;
		n |= bool(_bit_ref(_tree.get(), *x + n));
	}

	return n;
}

_lowbits_base::_lowbits_base(size_type input_size)
{
	// Step 0: Generate trei.

	// Step 0.0: Calculate trei size (in bits)
	size_type bucket_size = 0;
	size_type level_size = input_size;

	while(level_size > 1)
	{
		_bucket_levels.push_back(bucket_size << 1);

		level_size = (level_size + 1) >> 1;
		bucket_size += level_size;
	}

	size_t tree_words = ((bucket_size << 1) + _word_bits - 1) / _word_bits;
	_tree.reset(new _word_type[tree_words]);
	std::fill(_tree.get(), _tree.get() + tree_words, 0);
}
