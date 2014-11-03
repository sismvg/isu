#ifndef ISU_BITMAP_HPP
#define ISU_BITMAP_HPP

#include <isu/win32/file.hpp>
#include <isu/gui/bitmap_core.hpp>
#include <isu/gui/image_header.hpp>
#include <isu/alg/more_for_each.hpp>
#include <isu/win32/lowercast_def.hpp>

namespace isu
{
	//½Ó¿Ú
	colour_pixle struct_cast(const RGB_quad& quad);
	class bitmap
		:public bitmap_core
	{
	public:
		bitmap();
		bitmap(const bitmap_info_header&);
		bitmap(const bitmap_info_header& header, const bitmap_core&);
		template<class Char>
		bitmap(const Char* name)
		{
			_construct(name);
		}
		template<class String>
		bitmap(const String& str)
		{
			_construct(str.c_str());
		}
		template<class String>
		void save(const String& str) const
		{
			save(str.c_str());
		}
		void save(const wchar_t*) const;
		const bitmap_file_header& file_header() const;
		const bitmap_info_header& info_header() const;
		const std::vector<RGB_quad>& palette() const;
		colour_pixle at(size_t height, size_t weight) const;
		colour_pixle at(const multi_index&) const;
		bool is_bottom_up() const;
		bool have_palette() const;
	private:
		void _construct_file_header(const bitmap_info_header&);
		void _construct(const wchar_t* file_name);
		byte* _construct_header(byte* begin, byte* end);
		byte* _read_palette(byte* begin, byte* end);
		bitmap_file_header _file_header;
		bitmap_info_header _info;
		std::vector<RGB_quad> _quad;
	};
}
#endif