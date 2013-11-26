#ifndef ISU_GO_THREAD_H
#define ISU_GO_THREAD_H

//File:thread.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/config/config.h>
#ifdef USE_BOOST_THREAD
#include <boost/thread.hpp>
#include <go/detail/context/context.h>
#include <go/detail/config/typedef_config.h>
#endif
namespace isu
{
	namespace go
	{
		namespace detail
		{
#ifdef USE_BOOST_THREAD
			class thread
				:public boost::thread
			{
			public:
				typedef boost::thread father_type;
				typedef context			context_type;
				context_type get_context();
				template<class Func>
				thread(Func f)
					:father_type(f)
				{

				}
				template<class Func,class ...Arg>
				thread(Func f, Arg... arg)
					: father_type(f, arg...)
				{

				}
				void set_context(const context_type&);
				void pause();
				void resume();
			};
#endif
		}
	}
}
#endif