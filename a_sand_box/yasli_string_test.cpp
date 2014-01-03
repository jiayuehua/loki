#include <loki/flex/flex_string.h>
#include <iostream>
struct B{};
struct A:public B
{
	enum A1{ A1 = 0};

};
int yasli_string_main()
{
	/*yasli::string s;
	std::cin>>s;
	std::cout<<s;*/
	std::cout<<sizeof(A)<<std::endl;
	return 0;
}