#include "test.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int main()
{
	std::vector<type> array_in, array_out;
	size_t scale, c, ops;
	size_t optotal, sortcount;

	sortcount = 0;
	optotal = 0;

	for(scale = 0; scale < 32u; scale++)
	{
		array_in.resize(1u << scale);
		array_out.resize(1u << scale);

		for(c = 0; c < (1u << scale); ++c)
			array_in[c] = type(c + 1);

		std::next_permutation(array_in.begin(), array_in.end());

		while(std::adjacent_find(array_in.begin(), array_in.end(), std::greater<type>()) != array_in.end())
		{
			ops = 0;
			lowbits_sort(array_in.begin(), array_in.end(), array_out.begin(), counted_less<type>(ops));
			optotal += ops;
			++sortcount;

			if(std::adjacent_find(array_out.begin(), array_out.end(), std::greater<type>()) != array_out.end())
			{
				std::cout << "Failure. Original sequence:" << std::endl;
				std::copy(array_in.begin(), array_in.end(), std::ostream_iterator<type>(std::cout, "\t"));
				std::cout << std::endl;

				std::cout << "New sequence:" << std::endl;
				std::copy(array_out.begin(), array_out.end(), std::ostream_iterator<type>(std::cout, "\t"));
				std::cout << std::endl;

				std::cout << "Ops: " << ops << std::endl;
				return 0;
			}

			std::next_permutation(array_in.begin(), array_in.end());
		}

		std::cout << optotal << ":" << sortcount << std::endl;
	}

	return 0;
}
