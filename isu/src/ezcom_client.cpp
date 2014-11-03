
#pragma once

#include <combaseapi.h>

#include <isu/win32/lowercast_def.hpp>
#include <isu/win32/lowercast_com_def.hpp>

#include <isu/win32/ezcom_client.hpp>

namespace isu
{
	namespace win32
	{
		const compent_info compent::normal_create(nullptr);

		compent_info::compent_info(
			iunknown* outer, dword cls_context, coserverinfo* server_info)
			:unknown_outer(outer), cls_context(cls_context), server_info(server_info)
		{

		}

		compent::compent(const string& prog_id, const compent_info& create_info)
		{
			if (SUCCEEDED(CLSIDFromProgID(prog_id.c_str(), &_object_clsid)))
			{
				if (FAILED(CoCreateInstance(
					_object_clsid, create_info.unknown_outer,
					create_info.cls_context, IID_IUnknown,
					reinterpret_cast<void**>(&_object) ) ) )
				{
					_object = nullptr;
				}
			}
		}

		interface_unknown* compent::object() const
		{
			return _object;
		}

		const clsid& compent::compent_clsid() const
		{
			return _object_clsid;
		}

	}
}