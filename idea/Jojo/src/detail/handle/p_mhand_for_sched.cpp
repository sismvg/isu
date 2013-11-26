
#include <go/detail/processor.h>
#include <go/detail/tools/utility.h>
#include <go/detail/handle/p_mgrhand_for_sched.h>

//File:p_mhand_for_sched
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

			p_mgrhand_for_sched::alloc_ret
				p_mgrhand_for_sched::allocp()
			{
					return allocp(machine_ptr());
			}

			p_mgrhand_for_sched::alloc_ret
				p_mgrhand_for_sched::allocp(const machine_ptr& m)
			{
				scoped_lock _lck(obj()->_mlock);
				auto& running = obj()->_mrunning;
				if (obj()->maxproc() > obj()->proc_size())
				{
					running.push_back(processor_ptr(new processor(m)));
					++obj()->_mnowproc;
					return running.back();
				}
				return alloc_ret();
			}

			void p_mgrhand_for_sched::setproc(const size_t max_proc)
			{
				obj()->maxproc(max_proc);
			}
		}
	}
}