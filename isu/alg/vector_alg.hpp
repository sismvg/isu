#ifndef ISU_ALG_VECTOR_ALG_HPP
#define ISU_ALG_VECTOR_ALG_HPP

namespace isu
{
	namespace alg
	{
		template<class Iter, class T,class Fn1,class Fn2>
		Iter near_binary_search(
			Iter begin, Iter end, const T& value, Iter boundary,
			Fn1 equal,Fn2 less)
		{
			typedef std::size_t size_type;
			typedef typename Iter::value_type value_type;
			size_type size = end - begin;
			size_type length = boundary - begin;
			if (size)
			{
				size_type middle = size / 2;
				while (true)
				{
					const value_type& middle_value = *(begin + middle);
					bool result = equal(middle_value, value);
					if (!result&&less(middle_value,value))
					{
						if (middle + 1 < length && less(value, *(begin + middle + 1)))
						{
							return begin + middle_value + 1;
						}
					}
					Iter middle_iter = begin + middle;
					if (result)
						return middle_iter;
					if (middle_value > value)
						return near_binary_search(begin, middle_iter, value, boundary,equal,less);
					else
						return near_binary_search(++middle_iter, end, value, boundary,equal,less);
				}
			}
			return end;
		}
		template<class Iter,class T,class Fn1,class Fn2>
		Iter near_binary_search(Iter begin,Iter end, const T& value,Fn1 equal,Fn2 less)
		{//±ÿ–Î∞¥’’…˝–Ú≈≈–Ú
			return near_binary_search(begin, end, value, end, equal, less);
		}

		template<class Con,class T>
		typename Con::iterator near_binary_search(Con& con, const T& value)
		{
			return near_binary_search(con.begin(), con.end(), value, std::equal_to<T>(), std::less<T>());
		}
	}
}
#endif