#ifndef ISU_GO_SRC_MGR_HANDLE_H
#define ISU_GO_SRC_MGR_HANDLE_H

//File:gosrc_mhand_for_p.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "access_handle.h"
#include <boost/function.hpp>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class go_srcmgr;
			class processor;
			class phand_for_g;
			ACCESS_HANDLE(gosrc_mhand_for_p, go_srcmgr, processor)
		private:
			friend class phand_for_g;
			typedef boost::function<function> functional;
			goroutine_ptr allocg(functional);
			void alloc_task(processor_ptr&,const bool);//分配任务
			void reciperare(srclist<shared_ptr<goroutine> >&);
			ACCESS_HANDLE_END
		}
	}
}
#endif