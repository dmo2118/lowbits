#include "test.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <memory>
#include <cstdlib>
#include <ctime>

int main()
{
	std::vector<type> array_in;
	unsigned long long frq = clock_ull_frq();

	std::srand(unsigned(std::time(NULL)));

	for(unsigned scale = 0; scale < 32; scale++)
	{
		std::size_t size = 1 << scale;
		unsigned long long sum = 0;
		unsigned n = 0;
		std::unique_ptr<type []> array_in(new type[size]);

		while(sum < frq / 2)
		{
			for(size_t i = 0; i != size; ++i)
				array_in[i] = std::rand() % (size * 4);

			unsigned long long then = clock_ull();
			lowbits<const type *> lb_sort(array_in.get(), size);
			for(unsigned i = size; i; --i)
				lb_sort();
			sum += clock_ull() - then;

			++n;
		}
		
		std::cout
			<< std::setw(14) << scale
			<< std::setw(14) << (1 << scale)
			<< std::setw(14) << n
			<< std::setw(14) << (double(sum * 1000u) / (n * frq))
			<< std::endl;
	}

	return 0;
}
