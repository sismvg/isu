#ifndef ISU_GO_GO_H
#define ISU_GO_GO_H

//File:go.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/config/config.h>
#include <go/detail/tools/mgrgeter.h>
#include <go/detail/schedule/schedule.h>

#pragma  comment(lib,"Jojo.lib")

namespace isu
{
	namespace go
	{
		class gotrans
		{
		public:
			typedef gotrans this_type;
			template<class Func>
			this_type& operator-(Func f)
			{
				detail::global_scheduler()->gogo(f);
				return *this;
			}
		};
		gotrans transer;
#define joimpl(md) isu::go::transer-[md]()
#define jo joimpl(&)
	}
}
#endif