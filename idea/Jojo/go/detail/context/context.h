#ifndef ISU_GO_CONTEXT_H
#define ISU_GO_CONTEXT_H

//File:context.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/config/config.h>
#include <windows.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			struct context
				: public CONTEXT
			{
				typedef CONTEXT father_type;
				context();
				context(const CONTEXT&);//need not explict
			};
		}
	}
}
#endif