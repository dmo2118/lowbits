#include "test.hpp"

#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

int main()
{
	std::vector<type> array_in;
	unsigned long long frq = clock_ull_frq();

	std::srand(unsigned(std::time(NULL)));

	for(unsigned scale = 0; scale < 32; scale++)
	{
		unsigned long long sum = 0;
		size_t props = 0;
		unsigned n = 0;
		array_in.resize(1u << scale);

		while(sum < frq / 2)
		{
			std::generate(array_in.begin(), array_in.end(), rand);

			unsigned long long then = clock_ull();
			lowbits_sort(array_in.begin(), array_in.end(), null_object(), counted_less<type>(props));
			sum += clock_ull() - then;

			++n;
		}
		
		std::cout
			<< std::setw(14) << scale
			<< std::setw(14) << (1 << scale)
			<< std::setw(14) << n
			<< std::setw(14) << (double(props) / n)
			<< std::setw(14) << (double(sum * 1000u) / (n * frq))
			<< std::endl;
	}

	return 0;
}
