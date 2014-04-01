
#ifndef ISU_ARCHIVE_CALL_HPP
#define ISU_ARCHIVE_CALL_HPP

#include <functional>
#include <boost/function.hpp>
namespace isu
{
	typedef unsigned char byte;
	template<class T>
	T* raw_pointer(T& value)
	{
		//�п���ĳ����������������&���ţ������п��ܲ��ܵõ���ȷ��ָ��
		return reinterpret_cast<T*>(reinterpret_cast<char&>(value));
	}

	template<class T>
	T& rarchive(byte*& bytes, std::size_t& length)
	{//����ȫ��ֻ֧��POD����
		assert(sizeof(T) <= length);
		T& value = *reinterpret_cast<T*>(bytes);
		bytes += sizeof(T); length -= sizeof(T);
		return value;
	}

	template<std::size_t count>
	class auto_call_impl
	{
	public:
		template<class Fn, class T, class... Type, class... Arg>
		static void impl_call(Fn fn, byte* bytes, std::size_t length, Arg... args)
		{
			T& value = isu::rarchive<T>(bytes, length);
			auto_call_impl<sizeof...(Type)>::impl_call<Fn, Type...>(fn, bytes, length, args..., value);
		}
	};

	template<>
	class auto_call_impl<1>
	{
	public:
		template<class Fn, class T, class... Arg>
		static void impl_call(Fn fn, byte* bytes, std::size_t length, Arg... args)
		{
			T& value = isu::rarchive<T>(bytes, length);
			fn(args..., value);
		}
	};

	//�Ҳ����õİ취�õ����������ǩ���������ڳ��õĺ�������Ҳ��stl��boost��
	//I can not find a good idea for get functional object sign
	//But we always use stl.functional boost.function or lambda so....
	template<class T, class... Arg>
	void auto_call(std::function<T(Arg...)> fn, byte* bytes, std::size_t length)
	{
		auto_call_impl<sizeof...(Arg)>::impl_call<std::function<T(Arg...)>, Arg...>(fn, bytes, length);;
	}

	template<class T, class... Arg>
	void auto_call(boost::function<T(Arg...)>& fn, byte* bytes, std::size_t length)
	{
		auto_call_impl<sizeof...(Arg)>::impl_call<boost::function<T(Arg...)>, Arg...>(fn, bytes, length);
	}
	
	template<class T, class... Arg>
	void auto_call(T(*fn)(Arg...), byte* bytes, std::size_t length)
	{
		auto_call_impl<sizeof...(Arg)>::impl_call< T(*)(Arg...), Arg...>(fn, bytes, length);
	}
}
#endif