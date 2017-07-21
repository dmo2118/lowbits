#include "lowbits.hpp"

namespace
{
	template<typename BoolIt, typename BoolItIt, typename Diff> inline Diff _descend(
		const BoolIt &b,
		const BoolItIt &first,
		BoolItIt x,
		Diff n)
	{
		while(x != first)
		{
			--x;
			n = (n << 1) | (Diff)b[*x + n];
		}

		return n;
	}
}

_lowbits_base::size_type _lowbits_base::_descend(std::vector<size_type>::const_iterator x, size_type n) const
{
	return ::_descend(_bit_bucket, _bucket_levels.begin(), x, n);
}

_lowbits_base::_lowbits_base(size_type input_size)
{
	// Step 0: Generate trei.

	// Step 0.0: Calculate trei size (in bits)
	size_type bucket_size = 0;
	size_type level_size = input_size;

	while(level_size != 1)
	{
		level_size = (level_size + 1) >> 1;
		bucket_size += level_size;
	}

	_bit_bucket.resize(bucket_size);
	_mark_area.resize(bucket_size);
}
