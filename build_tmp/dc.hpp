#ifndef ISU_DC_HPP
#define ISU_DC_HPP

#include <array>

#include <windows.h>
#include <isu/win32/lowercast_def.hpp>

namespace isu
{
	namespace gui
	{
		class gdi_object
		{
		public:
			typedef void* hobject;
			gdi_object(hobject obj = NULL);
			operator hobject();
		private:
			hobject _obj;
		};
		class dc
		{
		public:
			dc()
				:_dc(NULL)
			{}
			dc(hwnd wnd)
				:_dc(GetDC(wnd))
			{}
			dc(hdc device)
				:_dc(device)
			{}
			~dc()
			{
				DeleteDC(_dc);
			}
			
			template<class... Object>
			std::array<gdi_object, sizeof...(Object)> select_object(Object... objects)
			{
				std::array<gdi_object, sizeof...(Object)> result;
				return _select_object(result, 0, objects...);
			}
			hdc native_handle() const
			{
				return _dc;
			}
			hdc replace_dc(hdc dc)
			{
				hdc result = _dc;
				_dc = dc;
				return result;
			}
		private:
			template<class Array,class T>
			Array _select_object(Array& array, std::size_t index, T obj)
			{
				array[index] = SelectObject(_dc, obj);
				return array;
			}
			template<class Array,class T,class... Object>
			Array _select_object(Array& array,std::size_t index,T obj, Object... objects)
			{
				array[index] = SelectObject(_dc, obj);
				return _select_object(array, ++index, objects...);
			}
			hdc _dc;
		};
	}
}
#endif