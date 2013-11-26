
#include <go/go_except.h>
#include <go/detail/runtime/thread/thread.h>
#include <go/detail/handle/mhand_for_sched.h>
#include <go/detail/runtime/machine/machine.h>

//File:mhand_for_sched.cpp
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
			void mhand_for_sched::pause()
			{
				obj()->_mthread->pause();
			}

			void mhand_for_sched::resume()
			{
				obj()->_mthread->resume();
			}

			void mhand_for_sched::gotog(
				const shared_ptr<goroutine>& g)
			{
				//没有实现
			}
				void mhand_for_sched::run()
			{
				if (obj()->state()==mrunable)
				{
					obj()->_state(mrunning);
					obj()->_mthread = new thread(machine::_start_machine, obj().get());
				}
				else
					throw isu::go::error_status<mstate>
									("machine not mrunable", obj()->_mstate);
			}

			mhand_for_sched::processor_ptr 
				mhand_for_sched::rebind(processor_ptr p)
			{
				std::swap(obj()->_mcurp, p);
				return p;
			}

			shared_ptr<goroutine> 
				mhand_for_sched::switchg(const shared_ptr<goroutine>& g)
			{
				return obj()->_switchg(g);
			}
		}
	}
}