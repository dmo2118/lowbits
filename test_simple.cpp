#define LOWBITS_NOISE 1

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
	static const size_t size = 64;

	unsigned long long then, now;
	type array_in[size], array_out[size];

	size_t ops;

	std::cout << "lowbits: " << size << std::endl;

	std::srand(unsigned(std::time(NULL)));
	std::generate(array_in, array_in + size, rand);

//	sort(array_in, array_in + size, less<type>());

//	copy(array_in, array_in + size, ostream_iterator<type>(cout, "\t"));
//	cout << endl;

	ops = 0;
	std::copy(array_in, array_in + size, array_out);
	std::sort(array_out, array_out + size, counted_less<type>(ops));
//	copy(array_out, array_out + size, ostream_iterator<type>(cout, "\t"));
//	cout << endl;
	std::cout << "Built-in sort: " << ops << std::endl;

	std::fill(array_out, array_out + size, 0);

	ops = 0;
	then = clock_ull();
	lowbits_sort(array_in, array_in + size, array_out, counted_less<type>(ops));
	now = clock_ull();
//	copy(array_out, array_out + size, ostream_iterator<type>(cout, "\t"));
//	cout << endl;
	std::cout << "Ops: " << ops << std::endl;

	std::cout << "Time: " << (1000 * (now - then) / clock_ull_frq()) << " ms" << std::endl;

	std::cout << (std::adjacent_find(array_out, array_out + size, std::greater<type>()) == array_out + size ?
		"It's sorted!" : "It's not sorted.") << std::endl;

	return 0;
}
