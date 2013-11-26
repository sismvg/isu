#ifndef ISU_GO_MGRCONFIG_H
#define ISU_GO_MGRCONFIG_H

//File:mgrconfig.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com
#include "config.h"

#ifdef USE_BOOST
#include <boost/function.hpp>
typedef void(functional)();
#else
#include <functional>
using function = std::function<T>
#endif

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class mgrconfig
			{
			public:
			//	typedef functional<void()> function;
				typedef functional function;
			};
		}
	}
}

#endif