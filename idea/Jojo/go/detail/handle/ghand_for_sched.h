#ifndef ISU_GO_HANDLE_GHAND_FOR_SCHED_H
#define ISU_GO_HANDLE_GHAND_FOR_SCHED_H

//File:ghand_for_sched.h
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
			class goroutine;
			class scheduler;

			ACCESS_HANDLE(ghand_for_sched,goroutine,scheduler)
		public:

			ACCESS_HANDLE_END
		}
	}
}
#endif