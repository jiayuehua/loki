#include <loki/ScopeGuard.h>
#include <loki/RefToValue.h>
#include <iostream>

int f(int & s)
{
	--s;
	return 0;
}
void foo( int &i)
{
	int s = 0;
//	Loki::ScopeGuard s = Loki::MakeGuard(&f,s);
	Loki::MakeGuard(&f, Loki::ByRef(s));
	//Loki::MakeGuard(&f, s);
	++s;
}
int scopeguard_main()
{
	int i = 100;
	const Loki::RefToValue<int> a (i);
	a+=2;
	std::cout<<i;
	return 0;

	/*int a = 100;
	foo(a);*/
}