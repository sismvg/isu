
//File:goroutine_base.cpp
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com
#include <tuple>
#include <windows.h>
#include <go/detail/runtime/goroutine/goroutine_base.h>


#define CREATE_FIBER(Func)\
	fiber = CreateFiber(0, Func,&arg)

namespace
{
	void _run_than_back(const isu::goroutine_base::functional& f, void* fiber)
	{
		f();
	}
}

namespace isu
{

		void SetFiberData(lpvoid fiber, lpvoid data)
		{
			char* ptr = reinterpret_cast<char*>(fiber);
			memcpy(ptr, &data, sizeof(lpvoid));
		}

		lpvoid GetFiberData(lpvoid fiber)
		{
			return *reinterpret_cast<lpvoid*>(fiber);
		}
	

	goroutine_base::~goroutine_base()
	{
		if (_mfiber) 	DeleteFiber(_mfiber);
	}

	void goroutine_base::handoff(const goroutine_base& g)
	{//直接切换到目标G,不删除Context
			//没有实现
	}

	void goroutine_base::start(functional f)
	{
		start(f,_mfiber);
	}

	void goroutine_base::start_than_kill(functional f, lpvoid fiber)
	{
		start_than_kill(f, _mfiber, fiber);
	}

	void goroutine_base::context_switch(this_type& g)
	{
		assert(g._mfiber);
		SwitchToFiber(g._mfiber);
	}

	void goroutine_base::start_than_kill
		(functional caller, lpvoid& fiber,lpvoid will_kill)
	{//运行时会把调用自己的Fiber给杀死
		assert(will_kill);
		auto arg = std::make_tuple(_start_ready(caller), caller,will_kill);
		typedef decltype(arg) arg_type;	
		auto Func = [](lpvoid param)
		{
			//必须复制到栈上面，否则Delete的时候会出错
			arg_type arg = *reinterpret_cast<arg_type*>(param);
			DeleteFiber(std::get<2>(arg) );
			_run_than_back(std::get<1>(arg) ,std::get<0>(arg) );
		};
		_run(CREATE_FIBER(Func) );
	}

	void goroutine_base::start_than_kill(functional f)
	{
		start_than_kill(f, _mfiber,GetCurrentFiber());
	}

	void goroutine_base::start
		(functional caller,lpvoid& fiber)
	{
		auto arg = std::make_pair(_start_ready(caller), caller);
		typedef decltype(arg) arg_type;
		auto Func=[](lpvoid param)
		{
			auto* arg = reinterpret_cast<arg_type*>(param);
			_run_than_back(arg->second, arg->first);
		};
		_run(CREATE_FIBER(Func) );
	}

	goroutine_base::lpvoid 
		goroutine_base::_start_ready(const functional& caller)
	{
		assert(!caller.empty());
		return  _covfiber();
	}

	void goroutine_base::_run(lpvoid fb)
	{
		if (fb == nullptr)
			throw std::runtime_error("have not source to create windows fiber");
		SwitchToFiber(fb);
	}

	goroutine_base::lpvoid goroutine_base::_covfiber()
	{
		if (!IsThreadAFiber())
			ConvertThreadToFiber(nullptr);
		return nullptr;
	}

#undef CREATE_FIBER
}