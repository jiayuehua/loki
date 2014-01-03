#include <iostream>
#include <cassert>
int new_main()
{
	char* p = new char[100];
	for(int i = 0; i<100; ++i)
	{
		
		std::cout<<i<<":";
		std::cout<<(int)p[i]<<std::endl;
		assert(p[i] == 0);
	}
	return 0;
}