
#pragma once

#include <atlbase.h>

#include "basic_event_proxy.hpp"

namespace isu
{
	namespace win32
	{
		template<class T, const GUID* EventGUID>
		using event_proxy = basic_event_proxy<T, ATL::IConnectionPointImpl<T, EventGUID>>;
	}
}