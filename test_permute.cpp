#include "test.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>

int main()
{
	for(size_t size = 1; size < 64; ++size)
	{
		size_t sortcount = 0;
		size_t ops_min = std::numeric_limits<size_t>::max(), ops_max = 0;
		unsigned long long ops_total = 0;

		std::unique_ptr<type []> array_in(new type[size]);

		for(size_t i = 0; i != size; ++i)
			array_in[i] = type(i + 1);

		do
		{
			size_t ops = 0;
			lowbits<const type *, counted_less> lb_sort(array_in.get(), size, ops);

			for(size_t i = 0; i != size; ++i)
			{
				if(array_in[*lb_sort] != i + 1)
				{
					std::cout << "Failure. Original sequence:" << std::endl;
					std::copy(array_in.get(), array_in.get() + size, std::ostream_iterator<type>(std::cout, " "));
					std::cout << std::endl;
					return EXIT_FAILURE;
				}

				++lb_sort;
			}

			ops_total += ops;
			if(ops > ops_max)
				ops_max = ops;
			if(ops < ops_min)
				ops_min = ops;

			++sortcount;
			std::next_permutation(array_in.get(), array_in.get() + size);
		}
		while(std::adjacent_find(array_in.get(), array_in.get() + size, std::greater<>()) != array_in.get() + size);

		std::cout <<
			size << '\t' <<
			ops_min << '\t' <<
			(double(ops_total) / sortcount) << '\t' <<
			ops_max << '\t' <<
			sortcount << std::endl;
	}

	return EXIT_SUCCESS;
}
