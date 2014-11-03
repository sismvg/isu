#ifndef ISU_PLATFORM_HPP
#define ISU_PLATFORM_HPP

#include <vector>

namespace isu
{
	namespace platform
	{
		template<class T>
		class stream_vector
			:public std::vector<T>
		{//高度耦合，且无法增长删除，只能访问和修改bit,不支持非纯ADT类型
		public:
			typedef std::vector<T> core;
			typedef typename core::iterator iterator;
		};
	}
}
#endif