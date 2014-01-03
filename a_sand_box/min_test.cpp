#include <iostream>
#include <cmath>
#include <loki_ext/MinMax.h>
#include <boost/type_traits.hpp>

struct B
{
	B& operator ++()
	{
		++b;
		return * this;
	}
	int b;
	B(int t):b(t){}
};

struct D:public B
{
	D& operator ++()
	{
		++d;
		return * this;
	}
	int d;
	D(int t):B(0),d(t){}
};
int foo(int & i)
{
	return i;
}
bool operator <(const B& b1, const D&d1 ){return b1.b<d1.d;}
bool operator <(const D& d1, const B&b1 ){return d1.d<b1.b;}
//
//B& foo1(const D& d1)
//{
//	return d1;
//}
B min(B& b1)
{
	return b1;
}
int min_test_main()
{
	B b1(100);
	int a = 100;
	const D d1(1);
	++b1;
	++b1;
	++b1;
	++b1;
	++b1;
	++Min(b1,d1);
	++min(b1);
	//++foo(a);
	std::cout<<b1.b<<" "<<d1.d<<"\n";
	//std::cout<<boost::is_same<MinMaxTraits<const B, const D>::Result,const B&>::value;
	//std::cout<<boost::is_same<MinMaxTraits<B, D>::Result, const D&>::value;
	//std::cout<<boost::is_same<MinMaxTraits<B, D>::Result, const B&>::value;
	//std::cout<<boost::is_same<MinMaxTraits<B, D>::Result, D&>::value;
	//std::cout<<boost::is_same<MinMaxTraits<B, const D>::Result, B&>::value;
	//std::cout<<boost::is_same<MinMaxTraits<B, const D>::Result, const D&>::value;
	//std::cout<<boost::is_same<MinMaxTraits<B, const D>::Result, const B&>::value;
	//std::cout<<boost::is_same<MinMaxTraits<B, const D>::Result, D&>::value;
	std::cout<<boost::is_same<MinMaxTraits<B, const D>::Result, B>::value;
	std::cout<<boost::is_same<MinMaxTraits<B, const D>::Result, const D>::value;
	std::cout<<boost::is_same<MinMaxTraits<B, const D>::Result, const B>::value;
	std::cout<<boost::is_same<MinMaxTraits<B, const D>::Result, D>::value;
	//B & b1 = d1;
	return 0;
}