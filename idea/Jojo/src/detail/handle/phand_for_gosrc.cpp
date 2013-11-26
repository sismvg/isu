
#include <go/detail/tools/utility.h>
#include <go/detail/handle/phand_for_gosrc.h>
#include <go/detail/runtime/processor/processor.h>

//File:phand_for_gosrc.cpp
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

			const bool phand_for_gosrc::alloc_freeg(goroutine_list& list)
			{
				scoped_lock _lck(obj()->_mlock);
				if (obj()->_mfreed.empty())
				{
					return false;
				}
				spliceback(list, obj()->_mfreed, 1);
				return true;
			}

			phand_for_gosrc::machine_ptr
				phand_for_gosrc::
					rebind_cureent_machine(const machine_ptr& m)
			{
				scoped_lock _lck(obj()->_mlock);
				auto ret = m;
				obj()->_mcurm = m;
				return ret;
			}

		}
	}
}