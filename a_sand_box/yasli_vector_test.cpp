#include <loki/yasli/yasli_vector.h>
#include <iostream>
#include <loki_ext/mojo.h>
#include <vector>
#include <cstring>
#include <algorithm>

int yasli_vector_test_main()
{
	std::ostream_iterator<int> oi(std::cout," ");
	yasli::vector<int> a(2);
	yasli::vector<int> b(3,10);
	std::copy(a.begin(),a.end(),oi);
	std::cout<<std::endl;
	a.insert(a.end(),b.begin(),b.end());
	std::copy(a.begin(),a.end(),oi);
	std::cout<<std::endl;
	int c = 100;
	std::cout<<"b size: "<<b.size()<<std::endl;
	b.move_back_nstd(c);
	b.move_back_nstd(c);
	b.move_back_nstd(c);
	std::cout<<"b size: "<<b.size()<<std::endl;
	std::copy(b.begin(),b.end(),oi);
	std::cout<<std::endl;
	return 0;
}