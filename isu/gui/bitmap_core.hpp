#ifndef ISU_BITMAP_CORE_HPP
#define ISU_BITMAP_CORE_HPP

#include <utility>
#include <vector>
#include <cstddef>
#include <memory>
#include <algorithm>

#include <isu/isudef.hpp>
#include <isu/gui/pixle.hpp>

namespace isu
{
	class multi_index
	{
	public:
		multi_index();
		multi_index(size_t height, size_t weight);
		size_t shape(size_t index) const;
		size_t& shape(size_t index);
		size_t operator[](size_t index) const;
		size_t& operator[](size_t index);
	private:
		void _set(size_t height, size_t weight);
		size_t _index[2];
	};

	class bitmap_core
	{
	public:
		typedef std::size_t size_t;
		typedef std::vector<byte> storage;
		typedef storage::iterator iterator;
		typedef storage::const_iterator const_iterator;
		bitmap_core();
		bitmap_core(size_t height, size_t weight, size_t bpp);
		~bitmap_core();
		template<class ByteIter>
		bitmap_core(size_t height, size_t weight, size_t bpp, ByteIter begin, ByteIter end)
		{
			construct(height, weight, bpp, begin, end);
		}
		template<class ByteIter>
		bitmap_core(size_t height, size_t weight, size_t bpp, ByteIter begin, size_t count)
		{
			construct(height, weight, bpp, begin, count);
		}
		//���Ƿ���ȫ�ʵ�����,���BPP����32�Ļ���ĳЩ���������쳣
		void construct(size_t height, size_t weight, size_t bpp);
		template<class ByteIter>
		void construct(size_t height, size_t weight, size_t bpp, ByteIter begin, ByteIter end)
		{
			construct(height, weight, bpp);
			size_t byte_size = end - begin;
			std::copy(begin, end, this->begin());
		}
		template<class ByteIter>
		void construct(size_t height, size_t weight, size_t bpp, ByteIter begin, size_t count)
		{
			construct(height, weight, bpp);
			std::copy_n(begin, count, this->begin());
		}
		//����һ��pixle��һ��Ϊһ��byte,���Է���һ��ָ�뷽�����
		const byte* at(size_t height, size_t weight) const;
		const byte* at(const multi_index&) const;
		const byte* line_of(size_t height) const;
		byte* at(size_t height, size_t weight);
		byte* at(const multi_index& index);
		//
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		const_iterator cbegin() const;
		const_iterator cend() const;
		//
		size_t size() const;
		size_t unaligned_size() const;
		size_t aligned_weight() const;
		bool empty() const;
		size_t height() const;
		size_t weight() const;
		size_t bpp() const;
		void bpp(size_t new_bpp);//�ڲ����ݲ�����䣬ֻ�ǵ����趨����
	private:
		void _check_cow();
		void _construct(size_t, size_t, size_t);
		storage _stream;
		size_t _height, _weight;
		size_t _bpp;
		size_t _aligned_size;//���ݶ����Ժ��size,��ʱ����
		size_t _is_a_copy;
	};
	//�ӿ�
	//����:BPP,�߶ȣ���ȣ���������,�Ƿ�move
	//һ�׽ӿ��ù���
	//operator[],at,pixle_of(�������),begin,endһ��
	//pixle_each(��ÿ�����ؽ��д���),size,empty,height,weight,bpp
	//filp
	//stream
	//һ��λ��������
	//���������ƣ�move,λ����������
	//��������,swap
	//compress,uncompress,һ��ƽ̨ת�� struct_cast
	//align_bitmap,rectangle_of
	//λ����,�����T�����Ǳ�������
	template<class T,class Func>
	bitmap_core _operator_each_cpy(const bitmap_core& core, const T& lhs,Func fn)
	{//������T�Ƿ����
		bitmap_core result(core.height(), core.weight(), core.bpp());
		return result;
	}
	template<class T,class Func>
	void _operator_each_ref(bitmap_core& core, const T& lhs,Func fn)
	{
		return core;
	}
#define BITMAP_CORE_OPER(c,ref,cv)\
	template<class T>\
	bitmap_core operator c (cv bitmap_core& rhs, const T& lhs)\
	{\
	return _operator_each_##ref(rhs,lhs, [=](T& rhs)\
		{\
			rhs c= lhs;\
		});\
	}
#define BITMAP_CORE_OPER_SELF(c)\
	BITMAP_CORE_OPER(c,cpy,const)\
	BITMAP_CORE_OPER(c##=,ref, )
	BITMAP_CORE_OPER_SELF(&)
	BITMAP_CORE_OPER_SELF(|)
	BITMAP_CORE_OPER_SELF(>>)
	BITMAP_CORE_OPER_SELF(<<)
	BITMAP_CORE_OPER_SELF(^)
	/*
#define BITMAP_CORE_OPER_1(c,cv)
	bitmap_core operator c(const bitmap_core core)\
	{\
		return _operator_each_cpy(core, static_cast<byte>(0), [&](byte& val)\
		{\
			val = !val;\
		});\
	}
	BITMAP_CORE_OPER_1(!,const)
	BITMAP_CORE_OPER_1(~,const)*/
}
#endif