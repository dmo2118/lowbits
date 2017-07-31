#include "lowbits.hpp"

_lowbits_base::size_type _lowbits_base::_descend(std::vector<_word_type *>::const_iterator x, size_type n) const
{
	while(x != _bucket_levels.begin())
	{
		--x;
		n <<= 1;
		n |= ((*x)[_word_offset(n)] & _word_mask(n)) != 0;
	}

	return n;
}

_lowbits_base::_lowbits_base(size_type input_size)
{
	// Step 0: Generate trei.

	// Step 0.0: Calculate trei size (in bits)

	{
		size_type tree_words = 0;
		size_type level_size = input_size;
		while(level_size > 1)
		{
			level_size = (level_size + 1) >> 1;
			tree_words += ((level_size << 1) + _word_bits - 1) / _word_bits;
		}

		_tree.reset(new _word_type[tree_words]);
		std::fill(_tree.get(), _tree.get() + tree_words, 0);
	}

	size_type tree_words = 0;
	size_type level_size = input_size;
	while(level_size > 1)
	{
		_bucket_levels.push_back(_tree.get() + tree_words);

		level_size = (level_size + 1) >> 1;
		tree_words += ((level_size << 1) + _word_bits - 1) / _word_bits; // TODO: Refactor with what's directly above.
	}
}
