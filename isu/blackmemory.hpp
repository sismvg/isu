#ifndef ISU_BLACKMEMORY_HPP
#define ISU_BLACKMEMORY_HPP

#include <isu/isudef.hpp>
#include <isu/alg/more_for_each.hpp>

namespace isu
{
	template<class ByteIter, class Func>
	void iter_advance(ByteIter& iter, size_t length, Func fn)
	{//Ҳ��Ӧ�÷ֵ�more_for_each��
		alg::index_each(0u, length, [&](size_t index)
		{
			fn(index, iter); ++iter;
		});
	}

	template<class T>
	void order_memcpy(T* dst, T* src, size_t size)
	{//��memcpy��ͬ����src������ʲô˳��dst��ʲô˳��
		//��Ҫ����һ���ڴ��copy��һ���ṹ��ӵ�е��ڴ��е�ʱ��
		for (size_t index = 0; index != size; ++index)
			dst[index] = src[index];
	}

	template<class ByteIter, class ByteType>
	ByteIter copy_to_buffer(ByteIter begin, ByteType* buffer, std::size_t size)
	{//����OutputIterator,�Լ����ms��std::copy��char��wchar_t�ľ���
		//˳�����ܷ��ظ�copy�Ժ�begin��λ��,���
		iter_advance(begin, size, [&](int index, ByteIter& iter)
		{
			buffer[index] = *iter;
		});
		return begin;
	}

	template<class ByteIter, class T>
	ByteIter cpynew(ByteIter begin, T& var)
	{//�����ݸ��Ƶ�������..ΪOutputIterator����
		//��֧��bool
		memcpy(&var, begin, sizeof(T));
		return begin + sizeof(T);
	}
}
#endif