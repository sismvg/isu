
#include <go/runtime_mgr.h>
#include <go/detail/tools/utility.h>
#include <go/detail/tools/mgrgeter.h>
#include <go/detail/handle/phand_for_gosrc.h>
#include <go/detail/handle/gosrc_mhand_for_p.h>

//File:g_mgrhand_for_p.cpp
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
			namespace
			{
				template<class T>
				void desemapore(T& val, std::size_t cnt)
				{
					for (std::size_t index = 0; index != cnt; ++index)
						val.wait();
				}
			}

			void gosrc_mhand_for_p::alloc_task(processor_ptr& proc,const bool wait)
			{
				if (wait) obj()->_mgcount.wait();
				else if (!obj()->_mgcount.try_wait()) return;

				scoped_lock _lck(obj()->_mlock);
				//这个时候被切换掉
				std::size_t size = obj()->_mrunableg.size();
				assert(size != 0);

				std::size_t max_proc = global_processor_manager()->maxproc();
				std::size_t alloc_gcount = max_proc>=size ? 1 : size / max_proc ;
				proc->addtask(obj()->_mrunableg, obj()->_mrunableg.begin(), alloc_gcount);
				desemapore(obj()->_mgcount, alloc_gcount - 1);
			}

			gosrc_mhand_for_p::goroutine_ptr
				gosrc_mhand_for_p::allocg(functional f)
			{
				return obj()->_allocg(f);
			}

			void gosrc_mhand_for_p::
				reciperare(srclist<shared_ptr<goroutine> >& list)
			{
				scoped_lock _lck(obj()->_mlock);
				obj()->_mrunableg.splice(obj()->_mrunableg.end(),list);
			}
		}
	}
}