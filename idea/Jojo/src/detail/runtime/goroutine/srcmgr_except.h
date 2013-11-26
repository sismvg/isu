#ifndef ISU_GO_DETAIL_SRCMGR_EXCEPT_H
#define ISU_GO_DETAIL_SRCMGR_EXCEPT_H

//File:srcmgr_except.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <stdexcept>
namespace isu
{
	namespace go
	{
		namespace detail
		{
			class too_much_goroutine : public std::runtime_error
			{
			public:
				too_much_goroutine() :std::runtime_error("have not memory source to create goroutine")
				{}
			};
		}
	}
}
#endif