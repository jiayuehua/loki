#include <iostream>

template <class T>
struct A{
	//friend std::ostream& operator<<(std::ostream & os, const A<T>& t);

};
template <class T>
std::ostream& operator<<(std::ostream & os, const A<T>& t)
{
	return	os<<"t";
}
int main()
{
	A<int> a;
	std::cout<<a;
	
}