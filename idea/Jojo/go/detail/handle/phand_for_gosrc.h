#ifndef ISU_GO_PHAND_FOR_GO_SRCMGR_H
#define ISU_GO_PHAND_FOR_GO_SRCMGR_H

//File:phand_for_gosrc.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "access_handle.h"
#include <cstddef>
#include <go/detail/runtime/processor/processor.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class processor;
			class go_srcmgr;
			class goroutine;

			ACCESS_HANDLE(phand_for_gosrc,processor,go_srcmgr)
		private:
			typedef srclist<goroutine_ptr> goroutine_list;
				const bool alloc_freeg(goroutine_list&);
				machine_ptr rebind_cureent_machine(const machine_ptr&);
			ACCESS_HANDLE_END
		}
	}
}
#endif