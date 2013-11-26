#ifndef ISU_mhand_for_gosrc_FOR_PROCESSOR
#define ISU_mhand_for_gosrc_FOR_PROCESSOR

//File:mhand_for_p.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "access_handle.h"
#include <go/detail/config/typedef_config.h>
namespace isu
{
	namespace go
	{
		namespace detail
		{
			class machine;
			class processor;
			ACCESS_HANDLE(mhand_for_p,machine,processor)
		private:
			void dahua();//此操作会打断goroutine
			ACCESS_HANDLE_END
		}
	}
}
#endif