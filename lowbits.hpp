// 426518

#ifndef LOWBITS_HPP
#define LOWBITS_HPP

#include <vector>
#include <iterator>

#if LOWBITS_NOISE >= 1
#define LOWBITS_NOISE1(x) x
#else
#define LOWBITS_NOISE1(x)
#endif

#if LOWBITS_NOISE >= 2
#define LOWBITS_NOISE2(x) x
#else
#define LOWBITS_NOISE2(x)
#endif

#if LOWBITS_NOISE
#include <iostream>
#endif

/*
__inline __fastcall unsigned bsr(unsigned x)
{
	__asm bsr eax, ecx;
}
*/

// FIXME: Step 1 doesn't work on initial run.

/*
unsigned lowbits_descend(std::vector<std::vector<bool>::iterator>::iterator first, 
						 std::vector<std::vector<bool>::iterator>::iterator x,
						 unsigned n)

*/

template<typename BoolIt, typename BoolItIt, typename Diff> inline Diff lowbits_descend(const BoolIt &b, const BoolItIt &first, BoolItIt x, Diff n)
{
	while(x != first)
	{
		--x;
		n = (n << 1) | (Diff)b[*x + n];
	}

	return n;
}

/* Potential optimizations:
   
   Remove mark area - probably can't do this.
   Optimize ascension so that there's a best match. */

/* Note: To do non power-of-two sorts, mask off the ends of the bit levels according to the bits of -input_size. */

template<typename RndIt, typename OutIt, typename Pred> OutIt lowbits_sort(RndIt first, RndIt last, OutIt x, Pred pr)
{
	LOWBITS_NOISE1(std::cout << "Sort hath begun." << std::endl);

	typedef typename std::iterator_traits<RndIt>::difference_type size_type;

	size_type input_size = last - first;

	// Step 0: Generate trei.
	
	// Step 0.0: Calculate trei size (in bits)

	std::vector<bool> bit_bucket, mark_area;

	{
		size_type bucket_size = 0;
		size_type level_size = input_size;

		while(level_size != 1)
		{
			level_size = (level_size + 1) >> 1;
			bucket_size += level_size;
		}

		bit_bucket.resize(bucket_size);
		mark_area.resize(bucket_size);
	}

	// Step 0.1: Populate the trei; gather stating bit locations.

	std::vector<size_type> bucket_levels;

	{
		size_type level_first;

		size_type bucket_size = 0, level_size = input_size;

		while(level_size != 1)
		{
			level_size = (level_size + 1) >> 1;
			level_first = bucket_size;

			bucket_size += level_size;

			for(size_type level_pt = 0; level_pt < level_size; ++level_pt)
			{
				bit_bucket[level_first + level_pt] = !pr(
					first[lowbits_descend(bit_bucket, bucket_levels.begin(), bucket_levels.end(), level_pt << 1)],
					first[lowbits_descend(bit_bucket, bucket_levels.begin(), bucket_levels.end(), (level_pt << 1) | 1)]);
			}

			bucket_levels.push_back(level_first);
		}
	}

	{
		typename std::vector<size_type>::iterator level_it(bucket_levels.end());
		size_type level_pt = 0;
		
		// Termination is a wild guess.
		LOWBITS_NOISE1(std::cout << "Step 1-4" << std::endl);

		for(;;)
		{
			// Step 1: Descend to location of lowest bit.

			// Step 1.1: Reset level pointers because I don't feel like tweaking the ascender...yet.
			level_it = bucket_levels.end();
			level_pt = 0;

			level_pt = lowbits_descend(bit_bucket, bucket_levels.begin(), level_it, level_pt);
			
			// Step 2: Output at location of bit.
			*x = first[level_pt];
			++x;

			level_it = bucket_levels.begin();
			level_pt >>= 1;

			// Step 3.0: Ascend until marked area is not empty.

			while(level_it != bucket_levels.end() && mark_area[*level_it + level_pt])
			{
				LOWBITS_NOISE2(std::cout << "x");

				++level_it;
				level_pt >>= 1;
			}

			if(level_it == bucket_levels.end()) // Termination goeth here (?)
				break;


			// Step 3.1: Flip a bit in the marked area, and ascend out.

			LOWBITS_NOISE2(std::cout << ";");

			mark_area[*level_it + level_pt].flip();
			bit_bucket[*level_it + level_pt].flip();

			++level_it;
			level_pt >>= 1;

			// Step 3.2: Flip bits & ascend until predicate is true.

			while(level_it != bucket_levels.end())
			{
				size_type pt0, pt1;

				pt0 = lowbits_descend(bit_bucket, bucket_levels.begin(), level_it, (level_pt << 1));
				pt1 = lowbits_descend(bit_bucket, bucket_levels.begin(), level_it, (level_pt << 1) | 1);

				LOWBITS_NOISE2(std::cout << pt0 << "," << pt1 << ":");

				if(!mark_area[*level_it + level_pt] && 
					bit_bucket[*level_it + level_pt] != !pr(
					first[pt0], 
					first[pt1]))
				{
					LOWBITS_NOISE2(std::cout << "1;");
					bit_bucket[*level_it + level_pt].flip();
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
		}
	}

	// Step 5: Done.

	LOWBITS_NOISE1(std::cout << "Sort hath ended." << std::endl);

	return x;
}

template<typename RndIt, typename OutIt> OutIt lowbits_sort(RndIt first, RndIt last, OutIt x)
{
	return lowbits_sort(first, last, x, std::less<typename std::iterator_traits<RndIt>::value_type>());
}

#endif
