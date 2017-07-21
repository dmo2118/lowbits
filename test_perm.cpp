#include "lowbits.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<type> array_in, array_out;
	size_t scale, c, ops;
	size_t optotal, sortcount;

	sortcount = 0;
	optotal = 0;

	for(scale = 0; scale < 32u; scale++)
	{
		array_in.resize(1u << scale);
		array_out.resize(1u << scale);

		for(c = 0; c < (1u << scale); ++c)
			array_in[c] = type(c + 1);

		next_permutation(array_in.begin(), array_in.end());

		while(adjacent_find(array_in.begin(), array_in.end(), greater<type>()) != array_in.end())
		{
			ops = 0;
			lowbits_sort(array_in.begin(), array_in.end(), array_out.begin(), counted_less<type>(ops));
			optotal += ops;
			++sortcount;

			if(adjacent_find(array_out.begin(), array_out.end(), greater<type>()) != array_out.end())
			{
				cout << "Failure. Original sequence:" << endl;
				copy(array_in.begin(), array_in.end(), ostream_iterator<type>(cout, "\t"));
				cout << endl;

				cout << "New sequence:" << endl;
				copy(array_out.begin(), array_out.end(), ostream_iterator<type>(cout, "\t"));
				cout << endl;

				cout << "Ops: " << ops << endl;
				return 0;
			}

			next_permutation(array_in.begin(), array_in.end());
		}

		cout << optotal << ":" << sortcount << endl;
	}

	return 0;
}
