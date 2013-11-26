
#include <go/detail/machine.h>
#include <go/detail/handle/mhand_for_p.h>
#include <go/detail/runtime/thread/thread.h>

//File:mhand_for_p.cpp
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
			void mhand_for_p::dahua()
			{
				obj()->_mthread->interrupt();
			}
		}
	}
}