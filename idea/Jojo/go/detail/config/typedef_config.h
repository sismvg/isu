#ifndef ISU_GO_TYPEDEF_CONFIG_H
#define ISU_GO_TYPEDEF_CONFIG_H

//File:typedef_config.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include	<atomic>
#include	<boost/shared_ptr.hpp>
#include	<go/detail/tools/srclist.h>
#include	<go/detail/config/config.h>
#include	<boost/smart_ptr/detail/spinlock.hpp>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class goroutine;
			class processor;
			class machine;

			template<class T>
			using shared_ptr = boost::shared_ptr<T>;

			struct typeconfig
			{
				typedef void(function)();
				typedef	shared_ptr<goroutine>	goroutine_ptr;
				typedef	shared_ptr<machine>		machine_ptr;
				typedef	shared_ptr<processor>	processor_ptr;
				typedef	srclist<goroutine_ptr>::iterator	goroutine_iterator;
				typedef	srclist<processor_ptr>::iterator	processor_iterator;
				typedef	srclist<machine_ptr>::iterator	machine_iterator;
				typedef	boost::detail::spinlock						lock_type;
				typedef	boost::detail::spinlock::scoped_lock	scoped_lock;
			};

		}
	}
}
#endif