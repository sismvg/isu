#include <go/channel/base_channel.h>
#include <windows.h>
#include <go/gohelper.h>
#include <go/detail_handle.h>

//File:base_channel.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

namespace
{
	typedef void* handle;
	handle current_thread_handle()
	{
		return GetCurrentThread();
	}

	handle true_thread_handle(handle h)
	{
		handle ret;
		handle curp = GetCurrentProcess();
		DuplicateHandle(curp, h, curp, &ret, DUPLICATE_SAME_ACCESS, TRUE, 0);
		return ret;
	}
	void block_current_thread()
	{
		SuspendThread(current_thread_handle());
	}

	const bool at_goroutine()
	{
		return isu::go::current_goroutine().get()!=nullptr;
	}

	void activate(handle thread)
	{
		ResumeThread(thread);
	}

	void activate(std::pair<handle,isu::go::detail::goroutine*>& g)
	{
		//这个实现有点麻烦
		activate(g.first);
	}

}
namespace isu
{
	base_channel::base_channel()
		:_msem(0)
	{

	}
	void base_channel::post()
	{
		_unblock();
	}

	void base_channel::wait()
	{
		_block();
	}

	void base_channel::_block()
	{
		if (at_goroutine())
		{
			_wait_goroutine();
		}
		else _msem.wait();
	}

	void base_channel::_wait_goroutine()
	{
		_msem.wait();
	}

	void base_channel::_unblock()
	{//不够优雅
	//	scoped_lock _lck(_mlock);
		at_goroutine() ? _post_to_goroutine() : _msem.post();
	}

	void base_channel::_post_to_goroutine()
	{
		_msem.post();
	}

}