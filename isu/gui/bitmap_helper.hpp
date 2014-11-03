#ifndef ISU_BITMAP_HELPER_HPP
#define ISU_BITMAP_HELPER_HPP

#include <isu/gui/bitmap.hpp>
#include <isu/win32/lowercast_def.hpp>
#include <isu/gui/bitmap_core_helper.hpp>

namespace isu
{
	//Func的参数
	//multi_index:当前像素相对于数据的坐标
	//height_with_windows:当前行相对于窗口的行,处理从下到上存放的DIB
	//colour_pixle:像素的颜色
	//color_index:当前像素转换为一个颜色表的索引,在BPP>=24的情况下无意义
	typedef
		std::function <
		void(
		const multi_index& index, size_t y,
		const colour_pixle& color, const dword& color_index)> pixle_proc;

	typedef std::function<void(int, int)> width_each_proc;

	void each_pixle(const bitmap&, pixle_proc, const rect_type* = nullptr);

	//这里的高度是相对与窗口的
	void line_each(const bitmap&, width_each_proc, std::size_t start_height, std::size_t end_height);

	void each_bits_pixle
		(const bitmap& map,const byte& block, 
		multi_index index, size_t y, pixle_proc fn, size_t callbit = 0);

	void each_pixle_impl_4(const bitmap&, pixle_proc, const rect_type&);

	void each_pixle_impl_16(const bitmap&, pixle_proc, const rect_type&);

	void each_pixle_impl_24(const bitmap&, pixle_proc, const rect_type&);

	const colour_pixle& colour_pixle_ref(const byte* bytes);

	void rectangle_of_impl(const bitmap& map, const rect_type& rect, pixle_proc);

	bitmap rectangle_of(const bitmap& map, const rect_type&, bool ttb = true);
}
#endif