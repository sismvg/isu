
#pragma once

#include <combaseapi.h>

#include <isu/win32/string.hpp>
#include <isu/win32/lowercast_def.hpp>
#include <isu/win32/lowercast_com_def.hpp>

namespace isu
{
	namespace win32
	{
		class compent;
		class compent_info
		{
		public:
			compent_info(
				iunknown* outer = nullptr,
				dword cls_context = CLSCTX_INPROC_SERVER,
				coserverinfo* server_info = nullptr);
			friend class compent;
		private:
			iunknown* unknown_outer;
			dword cls_context;
			coserverinfo* server_info;
		};

		class compent
		{
		public:
			compent(const string&, const compent_info& = normal_create);
			interface_unknown* object() const;
			const clsid& compent_clsid() const;
			static const compent_info normal_create;
		private:
			interface_unknown* _object;
			clsid _object_clsid;
		};

	}
}