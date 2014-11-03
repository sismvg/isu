#ifndef ISU_BITMAP_CORE_HELPER_HPP
#define ISU_BITMAP_CORE_HELPER_HPP

#include <functional>

#include <isu/gui/bitmap_core.hpp>
#include <isu/win32/lowercast_def.hpp>
namespace isu
{
	typedef std::function<void(const multi_index&)> byte_each_proc;
	template<class Func>
	void bitmap_line_each(int begin, int end, int setp, bool ttb, std::function<void(int)>);
	void rectangle_of_impl(const bitmap_core& core, const rect_type& rect, bool ttb,byte_each_proc);
	bitmap_core rectangle_of(const bitmap_core& core, const rect_type&, bool ttb = true);
}
#endif