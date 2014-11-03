#pragma once

#include <Unknwn.h>
#include <combaseapi.h>
#include <comdef.h>

namespace isu
{
	namespace win32
	{
#define COMCALL __stdcall

		typedef GUID guid;
		typedef CLSID clsid;
		typedef IID iid;
		typedef IClassFactory interface_factory;
		typedef IUnknown interface_unknown;

	}
}
