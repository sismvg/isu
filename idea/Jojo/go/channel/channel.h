#ifndef ISU_CHANNEL_H
#define ISU_CHANNEL_H

//File:channel.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <channel/base_channel.h>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace isu
{
	namespace detail
	{
		template<class T, class Alloc = std::allocator<T> >
		using channel_container = std::vector<T, Alloc>;

		template <class T, class Alloc = std::allocator<T> >
		class channel
			:public base_channel
		{//有buf
		public:
			typedef base_channel::lock_type lock_type;
			typedef base_channel::scoped_lock scoped_lock;

			void push_back(const T& val)
			{
				scoped_lock _lck(_mlock);
				_mbuf.push_back(val);
				post();
			}

			void pop_back()
			{
				scoped_lock _lck(_mlock);
				_pop_back();
			}

			const std::size_t signal_size() const
			{
				return _mbuf.size();
			}

			template<class DestType>
			void assign(DestType& val)
			{
				wait();//可能被抢占
				scoped_lock _lck(_mlock);
				_assign(val);
			}
		private:
			void _pop_back()
			{
				_mbuf.pop_back();
			}

			template<class DestType>
			void _assign(DestType& val)
			{
				val = _mbuf.back();
				_pop_back();
			}
			lock_type _mlock;
			channel_container<T, Alloc> _mbuf;
		};
	}



	template<class T>
	using shared_ptr = boost::shared_ptr<T>;

	template<class T,class Alloc = std::allocator<T> >
	class channel
		:private shared_ptr<detail::channel<T,Alloc> >
	{
		typedef detail::channel<T, Alloc> channel_type;
		typedef shared_ptr<channel_type> father_type;
		typedef channel this_type;
	public:
		channel()
			:father_type(new channel_type() )
		{

		}

		const std::size_t signal_size() const
		{
			return (*this)->signal_size();
		}

		friend this_type& operator<<(this_type& lhs, const T& rhs)
		{
			lhs->push_back(rhs);
			return lhs;
		}

		friend this_type& operator>>(this_type& lhs, T& rhs)
		{
			lhs->assign(rhs);
			return lhs;
		}
	};


}
#endif