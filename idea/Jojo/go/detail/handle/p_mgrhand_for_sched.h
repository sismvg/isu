#ifndef ISU_P_MGRHAND_FOR_GOSRC_H
#define ISU_P_MGRHAND_FOR_GOSRC_H

//File:p_mgrhand_for_sched.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "access_handle.h"
#include <go/detail/config/typedef_config.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class processor_mgr;
			class scheduler;

			ACCESS_HANDLE(p_mgrhand_for_sched, processor_mgr, scheduler)
			typedef processor_ptr alloc_ret;
			typedef std::size_t size_t;
		private:
			alloc_ret	allocp();
			alloc_ret	allocp(const machine_ptr&);
			void			setproc(const size_t);
			ACCESS_HANDLE_END
		}
	}
}
#endif