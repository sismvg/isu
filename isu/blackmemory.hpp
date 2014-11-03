#ifndef ISU_BLACKMEMORY_HPP
#define ISU_BLACKMEMORY_HPP

#include <isu/isudef.hpp>
#include <isu/alg/more_for_each.hpp>

namespace isu
{
	template<class ByteIter, class Func>
	void iter_advance(ByteIter& iter, size_t length, Func fn)
	{//也许应该分到more_for_each里
		alg::index_each(0u, length, [&](size_t index)
		{
			fn(index, iter); ++iter;
		});
	}

	template<class T>
	void order_memcpy(T* dst, T* src, size_t size)
	{//与memcpy不同的是src中数据什么顺序，dst就什么顺序
		//主要用于一个内存块copy到一个结构所拥有的内存中的时候
		for (size_t index = 0; index != size; ++index)
			dst[index] = src[index];
	}

	template<class ByteIter, class ByteType>
	ByteIter copy_to_buffer(ByteIter begin, ByteType* buffer, std::size_t size)
	{//用于OutputIterator,以及规避ms的std::copy对char和wchar_t的警告
		//顺带还能返回个copy以后begin的位置,多好
		iter_advance(begin, size, [&](int index, ByteIter& iter)
		{
			buffer[index] = *iter;
		});
		return begin;
	}

	template<class ByteIter, class T>
	ByteIter cpynew(ByteIter begin, T& var)
	{//把数据复制到变量里..为OutputIterator做的
		//不支持bool
		memcpy(&var, begin, sizeof(T));
		return begin + sizeof(T);
	}
}
#endif