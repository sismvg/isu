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
		{//�߶���ϣ����޷�����ɾ����ֻ�ܷ��ʺ��޸�bit,��֧�ַǴ�ADT����
		public:
			typedef std::vector<T> core;
			typedef typename core::iterator iterator;
		};
	}
}
#endif