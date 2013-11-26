#ifndef ISU_GO_THREAD_FORWARD_H
#define ISU_GO_THREAD_FORWARD_H

//File:thread_forward.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com
#include <go/detail/context/context.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			struct thread_forward
			{
				typedef context context_type;
			};
		}
	}
}
#endif