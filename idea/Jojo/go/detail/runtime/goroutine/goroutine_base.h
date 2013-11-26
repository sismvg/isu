#ifndef ISU_GOROUTINE_BASE_H
#define ISU_GOROUTINE_BASE_H

//File:goroutine_base.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <boost/function.hpp>
#include <stdexcept>

namespace isu
{
	typedef void* lpvoid;
	void		SetFiberData(lpvoid fiber, lpvoid data);
	lpvoid	GetFiberData(lpvoid fiber);

	class source_less
		:public std::bad_alloc
	{
	public:
		source_less()
		{	}
	};

		class goroutine_base
		{
		public:
			typedef boost::function<void()> functional;
			typedef void* lpvoid;
			typedef goroutine_base this_type;
			~goroutine_base();
			void handoff(const goroutine_base&);
			void start(functional);
			void start_than_kill(functional);
			static void context_switch(this_type&);
		protected:	
			static void start(functional, lpvoid&);
			static void start_no_kill(functional, lpvoid&);
			static void start_than_kill(functional, lpvoid&, lpvoid);
			void start_than_kill(functional, lpvoid);
			static lpvoid _covfiber();
			static lpvoid _start_ready(const functional&);
			static void _run(lpvoid);
			lpvoid _mfiber;
		};
}
#endif