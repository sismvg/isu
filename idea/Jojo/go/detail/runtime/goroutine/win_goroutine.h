#ifndef ISU_WIN_GOROUTINE
#define ISU_WIN_GOROUTINE

//File:win_goroutine.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "goroutine_base.h"
#include <windows.h>
namespace isu
{
	template<class FuncSign>
	class goroutine
		:public goroutine_base
	{
	public:	
		typedef void*			lpvoid;
		typedef goroutine		this_type;
		typedef FuncSign		funcational_sign;
		typedef boost::function<funcational_sign> caller;
		template<class sign>
		goroutine(const sign& f)
			:_mstart(f)
		{

		}

		~goroutine()
		{
		}
		template<class... Arg>
		void operator()(Arg... arg)
		{
			auto call=boost::bind(_mcall, arg...);
			goroutine_base::start(call);
		}

		void operator()()
		{
			goroutine_base::start(_mstart);
		}

		void start_than_kill()
		{
			goroutine_base::start_than_kill(_mstart);
		}

		const bool empty() const
		{
			return _mstart.empty();
		}

		void rebind(const caller& f)
		{
			_mstart = f;
		}

		void handoff(this_type& fiber)
		{
			//没有实现
		}

		void switchc(this_type& fiber)
		{
			fiber();
		}

		void switchc_kill(this_type& fiber)
		{
			fiber.start_than_kill();
		}

		const bool runed() const
		{
			return _mfiber != nullptr;
		}
		void yield_to(goroutine& g)
		{
			if (!g.runed())
			{
				//g没有运行，则必须让他初始化,而且不能杀死调用者
				g();
			}
			else
			{
				//g已经是运行的,那么只能单纯的switch过去
				context_switch(g);
			}
		}

		lpvoid fiber()
		{
			return _mfiber;
		}
	private:
		caller _mstart;
		void _start_than_kill(lpvoid fiber)
		{
			goroutine_base::start_than_kill(_mstart,fiber);
		}
	};
}
#endif