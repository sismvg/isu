
#include <isu/win32/dc.hpp>

namespace isu
{
	namespace gui
	{
		gdi_object::gdi_object(hobject obj)
			:_obj(obj)
		{}
		gdi_object::operator hobject()
		{
			return _obj;
		}
	}
}
