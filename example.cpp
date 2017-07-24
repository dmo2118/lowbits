#include "lowbits.hpp"

#include <iostream>

template<typename T, size_t N> constexpr size_t arraysize(T (&)[N])
{
	return N;
}

int main()
{
	// The source data, naturally.
	int source[] = {2, 1, 0, 4, 6, 3, 7, 5};

	// The Lowbits sort engine.
	lowbits<const int *> lb_sort(source, arraysize(source));

	for(unsigned i = arraysize(source); i; --i)
	{
		// A lowbits object is iterator-like. Dereferencing returns the
		// index of the current array element in the sorted output
		// sequence.
		std::cout << source[*lb_sort] << ' ';

		// Incrementing moves to the next element.
		++lb_sort;
	}

	std::cout << std::endl;

	// Output should be: "1 2 3 4 5 6 7 8 \n"

	return 0;
}
