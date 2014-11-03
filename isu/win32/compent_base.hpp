
#pragma once

#include <isu/win32/lowercast_def.hpp>
#include <isu/win32/lowercast_com_def.hpp>

namespace isu
{
	namespace win32
	{
		//声明了一个组件所必需的拥有的函数
		bool compent_can_unload_now();
		hresult compent_register_server(hinstance);
		hresult compent_unregister_server();
	}
}