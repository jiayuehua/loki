#ifndef DEBUG
#define DEBUG
#endif
#include <loki_ext/asserter.h>
#include <string>
int assert_test_main()
{
	for (int i = 0; i<10; ++i)
	{
		ASSERT(false);
	}
	ASSERT(false);
	return 0;
}