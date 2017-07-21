#define LOWBITS_NOISE 1

#include "lowbits.hpp"

#include <algorithm>
#include <iostream>
#include <functional>
#include <ctime>
#include <cstdlib>

using namespace std;

// 1234
// 1243
// 1324
// 1342
// 1423
// 1432

int main()
{
	static const size_t size = 64;

	unsigned __int64 then, now;
	type array_in[size], array_out[size];

	size_t ops;

	cout << "lowbits: " << size << endl;

	srand(unsigned(time(NULL)));
	generate(array_in, array_in + size, rand);

//	sort(array_in, array_in + size, less<type>());

//	copy(array_in, array_in + size, ostream_iterator<type>(cout, "\t"));
//	cout << endl;

	ops = 0;
	copy(array_in, array_in + size, array_out);
	sort(array_out, array_out + size, counted_less<type>(ops));
//	copy(array_out, array_out + size, ostream_iterator<type>(cout, "\t"));
//	cout << endl;
	cout << "Built-in sort: " << ops << endl;

	fill(array_out, array_out + size, 0);

	ops = 0;
	then = rdtsc();
	lowbits_sort(array_in, array_in + size, array_out, counted_less<type>(ops));
	now = rdtsc();
//	copy(array_out, array_out + size, ostream_iterator<type>(cout, "\t"));
//	cout << endl;
	cout << "Ops: " << ops << endl;

	cout << "Time: " << ((__int64)(now - then) / 450000.0) << endl;

	cout << (adjacent_find(array_out, array_out + size, greater<type>()) == array_out + size ? 
		"It's sorted!" : "It's not sorted.") << endl;

	return 0;
}
