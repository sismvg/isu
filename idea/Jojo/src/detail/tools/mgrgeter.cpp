
#include <go/runtime_mgr.h>
#include <go/detail/tools/mgrgeter.h>

//File:mgrgeter.cpp
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
			struct mgrpack
				:private typeconfig
			{
				mgrpack(std::size_t arg) :
				mprocmgr(new processor_mgr(arg) ),
				mmachine_mgr(new machine_mgr),
				msrcmgr(new go_srcmgr),
				mscheduler(new scheduler)
				{
				}

				~mgrpack()
				{
					mprocmgr->interrupt();
				}

				shared_ptr<processor_mgr> mprocmgr;	
				shared_ptr<machine_mgr>	mmachine_mgr;
				shared_ptr<go_srcmgr>		msrcmgr;
				shared_ptr<scheduler> mscheduler;
			};

			std::size_t _init_mgrpack()
			{
				SYSTEM_INFO info;
				ZeroMemory(&info, sizeof(info));
				GetSystemInfo(&info);
				return info.dwNumberOfProcessors;
			}

			mgrpack& global_manager_pack()
			{
				static mgrpack mgr(_init_mgrpack());
				return mgr;
			}

			shared_ptr<scheduler> global_scheduler()
			{
				return global_manager_pack().mscheduler;
			}

			shared_ptr<go_srcmgr> global_goroutine_manager()
			{
				return global_manager_pack().msrcmgr;
			}

			shared_ptr<processor_mgr> global_processor_manager()
			{
				return global_manager_pack().mprocmgr;
			}

			shared_ptr<machine_mgr> global_machine_manager()
			{
				return global_manager_pack().mmachine_mgr;
			}
		}
	}
}