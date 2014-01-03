#include <loki/yasli/yasli_vector.h>
#include <loki/yasli/yasli_fill_iterator.h>
#include <algorithm>
#include <iostream>
#include <iterator>
int fill_iterator_main()
{
	yasli::vector<int> v;
	int a = 100;
	yasli_nstd::fill_iterator<const int> b(100);
	yasli_nstd::fill_iterator<const int> e(100,3);
	v.insert(v.end(), b, e );
	std::ostream_iterator<int> oi(std::cout, " ");
	std::copy(v.begin(),v.end(), oi );
	return 0;
}