#ifndef ISU_GO_MACHINE_HANDLE_H
#define ISU_GO_MACHINE_HANDLE_H

//File:mhand_for_g.h
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
			class machine;
			class goroutine;

			ACCESS_HANDLE(mhand_for_g,machine,goroutine)
		private:
				//暂时没有需要的
			goroutine_ptr switchg(const goroutine_ptr&);
			void setg(const goroutine_ptr&);
			void suppend();
			void deatch();
			 void resume();
			ACCESS_HANDLE_END
		}
	}
}
#endif