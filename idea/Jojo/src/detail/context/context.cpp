#include <go/detail/context/context.h>

//File:context.cpp
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

namespace isu
{
	namespace go
	{
		namespace detail
		{
			context::context()
			{
				ZeroMemory(this, sizeof(CONTEXT) );
			}
			context::context(const CONTEXT& con)
				:father_type(con)
			{

			}
		}
	}
}