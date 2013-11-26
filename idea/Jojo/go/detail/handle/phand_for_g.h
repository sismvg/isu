#ifndef ISU_GO_PROCESSOR_HANDLE_H
#define ISU_GO_PROCESSOR_HANDLE_H

//File:phand_for_g.h
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
			class machine;

			ACCESS_HANDLE(phand_for_g,processor,goroutine)
			public:
				goroutine_ptr allocg(const bool =true);
				goroutine_ptr allocg_nowait();
				void runableg(const goroutine_ptr&);
				void freeg(const goroutine_ptr&);
			private:
				goroutine_ptr _splice_result();
				void _alloc_to_other();
			ACCESS_HANDLE_END
		}
	}
}
#endif