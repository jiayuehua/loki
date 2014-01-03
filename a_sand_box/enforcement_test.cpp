#include <loki_ext/enforce.h>
struct Node
{
	Node * next;
	int data;
};
#include <iostream>
int enforcement_main()
{
	try
	{
		Node* p = 0;
		(ENFORCE(p)("the world is not enough! "))->data;
	}
	catch (std::exception& e)
	{
		std::cout<<e.what();
	}
	return 0;
}