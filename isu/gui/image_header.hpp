#ifndef ISU_IMAGE_HEADER_HPP
#define ISU_IMAGE_HEADER_HPP

#include <cstddef>

#include <windows.h>

#include <isu/platform.hpp>
#include <isu/iterator_helper.hpp>

namespace isu
{
	typedef tagBITMAPFILEHEADER bitmap_file_header;
	typedef tagBITMAPCOREHEADER bitmap_core_header;
	typedef tagBITMAPCOREINFO bitmap_core_info;
	typedef BITMAPINFOHEADER bitmap_info_header;
	typedef RGBQUAD RGB_quad;
}
#endif