#ifndef ISU_MATI_HPP
#define ISU_MATI_HPP

#include <utility>
#include <vector>
#include <functional>
#include <type_traits>

namespace isu
{
	template<class Func>
	class result_of
	{//boost和stl的轮子不知道怎么用
	public:
		typedef typename Func::result_type type;
	};

	template<class T, class... Arg>
	class result_of<T(Arg...)>
	{
	public:
		typedef T type;
	};

	template<class T,class... Arg>
	class result_of < T(*)(Arg...)>
	{
	public:
		typedef T type;
	};

	class nop_class{};
	template<class T>
	class mati_impl_vec
		:private std::vector<T>
	{
		typedef std::vector<T> vec_t;
	public:
		typedef typename vec_t::iterator iterator;
		typedef typename vec_t::const_iterator const_iterator;
		typedef typename vec_t::reverse_iterator reverse_iterator;
		typedef typename vec_t::const_reverse_iterator const_reverse_iterator;

		using vec_t::begin;	using vec_t::end;
		using vec_t::cbegin;	using vec_t::cend;
		using vec_t::rbegin;	using vec_t::rend;
		using vec_t::crbegin;	using vec_t::crend;
		using vec_t::back; using vec_t::front;
		using vec_t::operator [];
	protected:
		using vec_t::push_back;
	};

	template<class T>
	typename T::cast_type cast_type_impl(std::true_type,const T* obj)
	{
		return const_cast<T&>(*obj);
	}
	template<class T>
	typename T::cast_type cast_type_impl(std::false_type,const T* obj)
	{
		return obj->back();
	}

	template<class Func>
	class myarg_type
	{
	public:
		typedef typename result_of<Func>::type result_type_raw;
		typedef typename std::_If<
			std::is_reference<result_type_raw>::value,
			std::reference_wrapper<typename std::remove_reference<result_type_raw>::type >,
			result_type_raw>::type result_type;
		typedef result_type type;
	};

	template<class T>
	class mylay
		:public mati_impl_vec<typename myarg_type<T>::type>
	{
	};
	template<class Func>
	class mati_impl
		:public std::_If<
		std::is_copy_assignable<typename myarg_type<Func>::type>::value,
		mylay<Func>,nop_class>::type
		//如何才能禁止mati_impl静态转换成nop_class or mati_impl_vec呢?
	{
	public:
		typedef mati_impl this_type;
		typedef typename myarg_type<Func>::type result_type;
		typedef std::integral_constant<
			bool,!std::is_copy_assignable<result_type>::value > is_void_result;
		typedef typename std::_If<is_void_result::value, this_type, result_type>::type cast_type;

		mati_impl(Func fn)
			:_fn(fn)
		{}
		cast_type& result()
		{
			return cast_type_impl(is_void_result(), this);
		}
		const cast_type& result() const
		{
			return const_cast<this_type*>(this)->result();
		}
		template<class... Arg>
		const mati_impl& operator()(Arg... arg) const
		{
			_do(is_void_result(),arg...);
			return *this;
		}

		static void* operator new(std::size_t) = delete;
		static void operator delete(void*) = delete;
	private:
		Func _fn;
		template<class... Arg>
		void _do(std::true_type, Arg... arg) const
		{
			_fn(arg...);
		}
		template<class... Arg>
		void _do(std::false_type, Arg... arg) const
		{
			const_cast<this_type*>(this)->push_back(_fn(arg...));
		}
	};
}

#define REPEAT(count) for(auto __jojo## count=count;__jojo## count !=0;--__jojo## count)

template<class Func>
class is_function_ptr
{
public:
	typedef std::false_type type;
	static const bool value = false;
};

template<class T,class... Arg>
class is_function_ptr<T(*)(Arg...)>
{
public:
	typedef std::true_type type;
	static const bool value = true;
};

template<class Func>
class fn_type
{
public:
	typedef typename std::add_pointer<Func>::type FuncPtr;
	typedef typename std::_If<is_function_ptr<FuncPtr>::value, FuncPtr, Func>::type type;
};

template<class Func>
isu::mati_impl<Func> impl_mati(Func fn, std::true_type)
{
	return isu::mati_impl<Func>(fn);
}

template<class Func>
isu::mati_impl<typename fn_type<Func>::type> impl_mati(Func fn, std::false_type)
{
	return isu::mati_impl<typename fn_type<Func>::type>(fn);
}

template<class Func>
auto mati(Func fn) ->decltype(impl_mati(fn,std::is_pointer<Func>::type() ) )
{//好吧，命名问题。。
	return impl_mati(fn,std::is_pointer<Func>::type());
}
#endif