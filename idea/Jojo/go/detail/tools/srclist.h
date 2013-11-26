#ifndef ISU_GO_SRCLIST_H
#define ISU_GO_SRCLIST_H

//File:srclist.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <list>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			template<class T,class Alloc=std::allocator<T> >
			using srclist = std::list<T, Alloc>;
			//一个比较简单的list,不过iterator支持deatch
		}
	}

}
#endif