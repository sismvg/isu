#ifndef ISU_FILE_HPP
#define ISU_FILE_HPP

#include <utility>
#include <memory>

#include <isu/isudef.hpp>

namespace isu
{
	std::pair<std::shared_ptr<byte>, size_t> loadfile(const wchar_t*);
}
#endif