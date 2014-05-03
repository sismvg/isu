
#include "defer.hpp"

namespace isu
{
	mydefer::mydefer()
	{

	}
	mydefer::~mydefer()
	{
		_fn();
	}
}
