
#include "defer.hpp"

namespace isu
{
	mydefer::mydefer()
	{

	}
	mydefer::~mydefer() noexcept
	{
		_fn();
	}
}
