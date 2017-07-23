#define LOWBITS_DEBUG 1

#include "test.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <ctime>

// 1234
// 1243
// 1324
// 1342
// 1423
// 1432

int main()
{
	static const size_t size = 47;

	std::cout << "lowbits: " << size << std::endl;

	std::srand(unsigned(std::time(NULL) + clock_ull()));

	type array_in[size], array_out[size];
	for(unsigned i = 0; i != size; ++i)
		array_in[i] = std::rand() % (size * 2);

//	sort(array_in, array_in + size, less<type>());

//	copy(array_in, array_in + size, ostream_iterator<type>(cout, "\t"));
//	cout << endl;

	size_t ops = 0;
	std::copy(array_in, array_in + size, array_out);
	std::sort(array_out, array_out + size, counted_less(ops));
//	copy(array_out, array_out + size, ostream_iterator<type>(cout, "\t"));
//	cout << endl;
	std::cout << "Built-in sort: " << ops << std::endl;

	std::fill(array_out, array_out + size, 0);

	ops = 0;
	unsigned long long then = clock_ull();
	lowbits_sort(array_in, array_in + size, array_out, counted_less(ops));
	unsigned long long now = clock_ull();
//	copy(array_out, array_out + size, ostream_iterator<type>(cout, "\t"));
//	cout << endl;
	std::cout << "Ops: " << ops << std::endl;

	std::cout << "Time: " << (1000.0 * (now - then) / clock_ull_frq()) << " ms" << std::endl;

	std::sort(array_in, array_in + size);
	bool ok = std::equal(array_in, array_in + size, array_out);
	std::cout << (ok ? "It's sorted!" : "It's not sorted.") << std::endl;

	return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
