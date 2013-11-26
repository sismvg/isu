
#include <go/detail_handle.h>
#include <go/detail/processor.h>
#include <go/detail/goroutine.h>
#include <go/detail/tools/mgrgeter.h>

//File:phand_for_g.cpp
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

namespace isu
{
	namespace go
	{
		namespace detail
		{

			typedef typeconfig::scoped_lock scoped_lock;

			processor::goroutine_ptr phand_for_g::allocg_nowait()
			{
				return allocg(false);
			}

			processor::goroutine_ptr
				phand_for_g::allocg(const bool wait)
			{//绝对不可能出现抢占
				auto& runable = obj()->_mrunable;
				if (runable.empty())
				{
					//从全局Mgr中请求数据
					scoped_lock _lck(obj()->_mlock);//for debugger
					auto handle = gosrc_mhand_for_p(global_goroutine_manager());
					handle.alloc_task(obj(),false);
					if (runable.empty())
					{
						_alloc_to_other();//从其他的P中找
					}
					if (runable.empty())
					handle.alloc_task(obj(), wait);
				}
				return _splice_result();
			}

			void phand_for_g::freeg(const goroutine_ptr& ptr)
			{
				scoped_lock _lck(obj()->_mlock);
				obj()->_mfreed.push_back(ptr);
			}

			void phand_for_g::runableg(const goroutine_ptr& g)
			{
				scoped_lock(obj()->_mlock);
				obj()->_mrunable.push_back(g);
			}

			phand_for_g::goroutine_ptr
				phand_for_g::_splice_result()
			{
				auto ret=
					obj()->_mrunable.empty() ?
					goroutine_ptr() :
					obj()->_mrunable.back();
				if (ret) obj()->_mrunable.pop_back();
				return ret;
			}

			void phand_for_g::_alloc_to_other()
			{
				for (processor_ptr& proc : *global_processor_manager().get() )
				if (proc.get() != obj().get())
				{
					if (proc->_alloc_half(*obj()))
						break;
				}
			}
		}
	}
}