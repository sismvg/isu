
#include <go/detail/handle/phand_for_sched.h>
#include <go/detail/runtime/processor/processor.h>

//File:phand_for_sched.cpp
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
			shared_ptr<goroutine> phand_for_sched::alloc_g()
			{
				auto ret = obj()->_mrunable.front();
				obj()->_mrunable.pop_front();
				return ret;
			}

			void phand_for_sched::pushg(const shared_ptr<goroutine>& g)
			{
				obj()->_mrunable.push_back(g);
			}

			const shared_ptr<machine>& 
				phand_for_sched::current_machine() const
			{
				return obj()->_mcurm;
			}

			shared_ptr<machine> 
				phand_for_sched::current_machine(
					shared_ptr<machine> m)
			{
				std::swap(obj()->_mcurm, m);
				return m;
			}
			const bool phand_for_sched::asking() const
			{//没有实现，所以挂起
				return true;
			}
		}
	}
}