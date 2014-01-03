int binary_search_min_element(int A[], int imin, int imax)
{
	if (imax < imin)
	{ return -1;}
	else if (imax == imin)
	{
		return imin;
	}
	else
	{
		int imid = (imin + imax)/2;

		if (A[imid] > A[imax])
		{
			if (imid+1 == imax)
			{
				++ imid;
			}
			
			return binary_search_min_element(A, imid, imax);
		}
		else if (A[imin] > A[imid])
		{
			if (imin+1 == imid)
			{
				++ imin;
			}
			return binary_search_min_element(A, imin, imid);
		}
		else
			return imin;
	}
}
template <int >
struct A{};
#include <algorithm>
#include <iostream>
#include "yasli_sort.hpp"
#include ""
int main()
{
	const	int i = 100;
	int b = 100;
	//std::cin>>b;
	const int j = b;
	/*++i;*/
	//A<i> a;
	int a[] = {5,6,7,8,9,0,1,2,3,4};
	YASTI::sort(a, a+ sizeof(a)/sizeof(int));
 //   int * p = a + binary_search_min_element(a,0,sizeof(a)/sizeof(int)-1) ;
	//std::rotate(a, p, a+sizeof(a)/sizeof(int));
	std::copy(a, a+sizeof(a)/sizeof(int), std::ostream_iterator<int>(std::cout," "));
}