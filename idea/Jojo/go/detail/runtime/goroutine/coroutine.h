#ifndef ISU_GO_COROUTINE_H
#define ISU_GO_COROUTINE_H

//File:coroutine.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/config/config.h>

#ifndef USE_BOOST_GOROUTINE
#include "win_goroutine.h"
#endif

namespace isu
{
	namespace go
	{
		namespace detail
		{
#ifdef USE_WIN_FIBER
			template<class FuncSign>
			class coroutine
				:public isu::goroutine<FuncSign>
			{
			public:
				typedef FuncSign functional_sign;
				typedef isu::goroutine<FuncSign> father_type;
				template<class sign>
				coroutine(const sign& f) :
					father_type(f)
				{}
			};
#endif
		}
	}
}
#endif