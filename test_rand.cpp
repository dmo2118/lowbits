#include "test.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>

int main(int argc, char **argv)
{
	std::minstd_rand r;

	double fac = argc > 1 ? std::atof(argv[1]) : 1.0;
	unsigned max_iter = fac * 512u;

	for(size_t size = 2; size != 512; ++size)
	{
		std::unique_ptr<unsigned []> array_in(new unsigned[size]), array_out(new unsigned[size]);

		double ln_size = log(size);
		for(unsigned n = std::max(1u, std::min(max_iter, unsigned(fac * 50000.0 / (size * ln_size * ln_size)))); n != 0; --n)
		{
			std::generate(
				array_in.get(),
				array_in.get() + size,
				std::bind(std::uniform_int_distribution<unsigned>(0, size * 2), std::ref(r)));

			lowbits_sort(array_in.get(), array_in.get() + size, array_out.get());
			std::sort(array_in.get(), array_in.get() + size);

			if(!std::equal(array_in.get(), array_in.get() + size, array_out.get()))
			{
				std::cout << "\nFailure. Original sequence:" << std::endl;
				std::copy(array_in.get(), array_in.get() + size, std::ostream_iterator<type>(std::cout, " "));
				std::cout << std::endl;
				return EXIT_FAILURE;
			}
		}

		std::cout << '.' << std::flush;
	}

	std::cout << std::endl;
	return EXIT_SUCCESS;
}
