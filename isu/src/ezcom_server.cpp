
#pragma once

#include <isu/win32/detail/Registry.h>
#include <isu/win32/ezcom_server.hpp>

namespace isu
{
	namespace win32
	{
		std::atomic<compent_server::count_type>
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

		class_factory_data::class_factory_data
			(const clsid& clsid, interface_factory* factory)
			:_factory_clsid(clsid),
			_factory(factory, [](interface_factory* factory)
		{
			factory->Release();
		}), _magic_cookie(0)
		{
		}

		class_factory_data::~class_factory_data()
		{
		}

		interface_factory* class_factory_data::factory() const
		{
			return _factory.get();
		}

		dword class_factory_data::cookie() const
		{
			return _magic_cookie;
		}

		hresult register_local_factory(class_factory_data& data)
		{
			hresult register_state =
				CoRegisterClassObject(
				data._factory_clsid, data.factory(),
				CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER,
				REGCLS_MULTI_SEPARATE, &data._magic_cookie);
			return register_state;
		}

		hresult unregister_local_factory(class_factory_data& data)
		{
			hresult register_state = CoRevokeClassObject(data._magic_cookie);
			return register_state;
		}

		hresult register_server(
			const clsid& classid, const string& compent_path, const string& progid,
			const string& description, const string& version)
		{
			return RegisterServer(
				classid, compent_path.c_str(), progid.c_str(),
				description.c_str(), version.c_str());
		}

		hresult unregister_server(const clsid& classid, const string& progid, const string& version)
		{
			return UnregisterServer(classid, progid.c_str(), version.c_str());
		}
	}
}