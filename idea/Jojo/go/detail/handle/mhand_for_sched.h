#ifndef ISU_GO_HANDLE_MHAND_FOR_SCHED_H
#define ISU_GO_HANDLE_MHAND_FOR_SCHED_H

//File:mhand_for_sched.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "access_handle.h"

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class machine;
			class scheduler;

			ACCESS_HANDLE(mhand_for_sched,machine,scheduler)
		public:
			void pause();
			void resume();
			void gotog(const shared_ptr<goroutine>&);
			void run();
			processor_ptr rebind(processor_ptr);
			shared_ptr<goroutine> switchg(const shared_ptr<goroutine>&);
			ACCESS_HANDLE_END
		}
	}
}
#endif