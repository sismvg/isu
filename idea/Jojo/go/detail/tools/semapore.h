#ifndef ISU_SEMAPORE_H
#define ISU_SEMAPORE_H

//File:semapore.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <cstddef>
#include <atomic>
#include <boost/smart_ptr/detail/spinlock.hpp>

namespace isu
{
	template<class T>
	struct max_value_of
	{
		enum{ value = static_cast<T>(-1) };
	};

	class semapore
	{
	public:
		typedef std::size_t count_type;
		semapore(const count_type, const count_type = max_value_of<count_type>::value);
		void wait();
		void desemapore( count_type);
		void post();
		void addsemapore( count_type);
		const bool try_wait();
		const bool timed_wait();
	private:
		boost::detail::spinlock _mlock;
		std::atomic<count_type> _mnc, _mmc;
	};
}
#endif