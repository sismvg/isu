#ifndef ISU_GO_GOROUTINE_HANDLE_H
#define ISU_GO_GOROUTINE_HANDLE_H

//File:goroutine_handle.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/config/typedef_config.h>
#include "access_handle.h"

namespace isu
{
	namespace go
	{
		namespace detail
		{
			typedef typeconfig::goroutine_ptr goroutine_handle;
		}
	}
}
#endif