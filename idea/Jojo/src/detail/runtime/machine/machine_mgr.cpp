
#include <go/runtime_mgr.h>
#include <go/detail/machine.h>
#include <go/detail/tools/utility.h>
#include <go/detail/tools/mgrgeter.h>

//File:machine_mgr.cpp
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
			const std::size_t machine_mgr::runing() const
			{
				return _mrunning.size();
			}
			const std::size_t machine_mgr::idleing() const
			{
				return _midle.size();
			}
			const std::size_t machine_mgr::syscall() const
			{
				return _msyscall.size();
			}
		}
	}
}