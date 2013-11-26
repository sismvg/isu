#ifndef ISU_GO_UTILITY_H
#define ISU_GO_UTILITY_H

//File:utility.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <iterator>

namespace isu
{
	namespace go
	{
		namespace detail
		{

			template<class List>
			void spliceback(List& lhs, List& rhs,
				typename List::iterator begin, typename List::iterator end)
			{
				lhs.splice(lhs.end(), rhs, begin, end);
			}

			template<class List>
			void spliceback(List& lhs, List& rhs,
				typename List::iterator begin, std::size_t cnt)
			{
	
				if (rhs.size() < cnt)
				{
					throw std::overflow_error("rhs.size()<cnt");
				}
				auto iter = begin;
				std::advance(iter, cnt);
				spliceback(lhs, rhs, begin, iter);
			}

			template<class List>
			void spliceback(List& lhs, List& rhs, std::size_t cnt)//在lhs插入rhs中的cnt个
			{
				spliceback(lhs, rhs, rhs.begin(), cnt);
			}

			template<class List>
			void spliceback(List& lhs, List& rhs)
			{
				spliceback(lhs, rhs, 1);
			}

			template<class List>
			typename List::iterator back_iter(List& lhs)
			{
				if (lhs.empty()) throw std::overflow_error("back_iter's arg ->lhs is empty");
				return --lhs.end();
			}

			template<class List>
			typename List::const_iterator back_iter(const List& lhs)
			{
				if (lhs.empty()) throw std::overflow_error("back_iter's arg ->lhs is empty");
				return --lhs.end();
			}

			template<class T, class Size>
			T advance(T begin, Size diff)
			{
				std::advance(begin, diff);
				return begin;
			}
		}
	}
}
#endif