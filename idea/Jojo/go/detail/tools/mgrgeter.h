#ifndef ISU_GO_MGRGETER_H
#define ISU_GO_MGRGETER_H

//File:mgrgeter.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/config/typedef_config.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class go_srcmgr;
			class processor_mgr;
			class machine_mgr;
			class scheduler;

			shared_ptr<go_srcmgr>			global_goroutine_manager();
			shared_ptr<processor_mgr>	global_processor_manager();
			shared_ptr<machine_mgr>		global_machine_manager();
			shared_ptr<scheduler>			global_scheduler();
		}
	}
}
#endif