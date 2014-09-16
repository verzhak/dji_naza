
#include "all.hpp"

int main(const int argc, const char * argv[])
{
	int ret = 0;

	amv_init();

	try
	{
		throw_if(argc != 2);

		const string fname = argv[1];

		// ############################################################################ 

		printf_TODO("IN %s\n", fname.c_str());
	}
	catch(...)
	{
		ret = -1;
	}

	amv_destroy();

	return ret;
}

