#include "stdafx.h"

#include <string>

#include <windows.h>
#include <combaseapi.h>

#include "Registry.h"

#include "compent.hpp"

compent_server::count_type 
	compent_server::_example_count = 0;

compent_server::count_type
	compent_server::add_example()
{
	return ++_example_count;
}

compent_server::count_type
	compent_server::release_example()
{
	return --_example_count;
}

bool compent_server::empty_example()
{
	return _example_count == 0;
}

HRESULT __stdcall RegisterCompent
	(LPCWSTR path, const GUID& guid,
	LPCWSTR prog_id, LPCWSTR desc, LPCWSTR ver)
{
	return RegisterServer(guid, path, prog_id, desc, ver);
}
HRESULT __stdcall UnRegisterCompent(const GUID& guid,LPCWSTR prog_id,LPCWSTR ver)
{
	return UnregisterServer(guid, prog_id, ver);
}