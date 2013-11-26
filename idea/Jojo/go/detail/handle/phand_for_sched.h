#ifndef ISU_GO_HANDLE_PHAND_FOR_SCHED_H
#define ISU_GO_HANDLE_PHAND_FOR_SCHED_H

//File:phand_for_sched.h
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
			class processor;
			class scheduler;
			ACCESS_HANDLE(phand_for_sched, processor, scheduler)
		public:	
			const bool asking() const;
			shared_ptr<goroutine> alloc_g();
			void pushg(const shared_ptr<goroutine>&);
			const shared_ptr<machine>& current_machine() const;
			shared_ptr<machine> current_machine(shared_ptr<machine>);
			ACCESS_HANDLE_END
		}
	}
}
#endif