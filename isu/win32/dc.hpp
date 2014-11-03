#ifndef ISU_DC_HPP
#define ISU_DC_HPP

#include <array>

#include <isu/win32/lowercast_def.hpp>
#include <isu/win32/shared_handle.hpp>

namespace isu
{
	namespace gui
	{
		class gdi_object
		{
		public:
			typedef HGDIOBJ hobject;
			gdi_object(hobject obj = NULL);
			operator hobject() const;
		private:
			hobject _obj;
		};
		class dc
			:public isu::win32::shared_dc
		{
		public:
			typedef isu::win32::shared_dc father_type;
			dc();
			dc(hdc, hwnd = NULL);
			template<class... Object>
			std::array<gdi_object, sizeof...(Object)> select_object(Object... objects)
			{
				std::array<gdi_object, sizeof...(Object)> result;
				return _select_object(result, 0, objects...);
			}
			hdc native_handle() const;
			hwnd wnd() const;
		private:
			hwnd _wnd;
			template<class Array,class T>
			Array _select_object(Array& array, std::size_t index, T obj)
			{
				array[index] = SelectObject(native_handle(), obj);
				return array;
			}
			template<class Array,class T,class... Object>
			Array _select_object(Array& array,std::size_t index,T obj, Object... objects)
			{
				array[index] = SelectObject(native_handle(), obj);
				return _select_object(array, ++index, objects...);
			}
		};
		template<class T>
		void delete_objects(const T& array)
		{
			std::for_each(array.begin(), array.end(), DeleteObject);
		}
	}
}
#endif