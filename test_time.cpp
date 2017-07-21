#include "lowbits.hpp"

#include <algorithm>
#include <vector>

#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
	vector<type> array_in;
	unsigned long long then, sum, frq = clock_ull_frq();
	unsigned scale, n;
	size_t props;

	srand(unsigned(time(NULL)));

	for(scale = 0; scale < 32; scale++)
	{
		sum = 0;
		props = 0;
		n = 0;
		array_in.resize(1u << scale);

		while(sum < frq / 2)
		{
			generate(array_in.begin(), array_in.end(), rand);

			then = clock_ull();
			lowbits_sort(array_in.begin(), array_in.end(), null_object(), counted_less<type>(props));
			sum += clock_ull() - then;

			++n;
		}
		
		cout 
			<< setw(14) << scale 
			<< setw(14) << (1 << scale) 
			<< setw(14) << n 
			<< setw(14) << (double(props) / n)
			<< setw(14) << (double(sum * 1000u) / (n * frq))
			<< endl;
	}

	return 0;
}
