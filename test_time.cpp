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
	unsigned __int64 then, sum;
	unsigned scale, props, n;

	srand(time(NULL));

	for(scale = 0; scale < 32; scale++)
	{
		sum = 0;
		props = 0;
		n = 0;
		array_in.resize(1u << scale);

		while(sum < PROCESSOR_MHZ / 2)
		{
			generate(array_in.begin(), array_in.end(), rand);

			then = rdtsc();
			lowbits_sort(array_in.begin(), array_in.end(), null_object(), counted_less<type>(props));
			sum += rdtsc() - then;

			++n;
		}
		
		cout 
			<< setw(14) << scale 
			<< setw(14) << (1 << scale) 
			<< setw(14) << n 
			<< setw(14) << ((double)(__int64)props / n) 
			<< setw(14) << ((double)(__int64)(sum * 1000u) / (n * (__int64)PROCESSOR_MHZ)) 
			<< endl;
	}

	return 0;
}
