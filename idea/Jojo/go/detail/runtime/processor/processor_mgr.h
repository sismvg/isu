#ifndef ISU_PROCESSOR_MGR_H
#define ISU_PROCESSOR_MGR_H

//File:processor_mgr.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "pstate.h"
#include <go/detail/config.h>


namespace isu
{
	namespace go
	{
		namespace detail
		{
			class processor;
			class p_mgrhand_for_sched;
			

			class processor_mgr
				:private typeconfig
			{
			public:

				typedef std::size_t									size_t;
				typedef srclist<processor_ptr>				processorlist;
				typedef typeconfig::processor_iterator	processor_iterator;

				explicit processor_mgr(const size_t);

				processor_iterator begin();
				processor_iterator end();
				const	std::size_t maxproc() const;
				void		maxproc(const std::size_t);
				const	std::size_t proc_size() const;
				void interrupt();

			private:
				lock_type _mlock;	
				processorlist _mrunning;
				std::atomic<size_t> _mmaxproc,_mnowproc;
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				friend class p_mgrhand_for_sched;
			};
		}
	}
}
#endif