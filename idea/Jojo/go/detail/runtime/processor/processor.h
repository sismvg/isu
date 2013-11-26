#ifndef ISU_GO_PROCESSOR_H
#define ISU_GO_PROCESSOR_H

//File:processor.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "pstate.h"
#include <go/detail/config/config.h>
#include <go/detail/config/typedef_config.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class machine;
			class goroutine;

			class phand_for_g;//g专用
			class phand_for_sched;//调度器专用
			class phand_for_gosrc;

			class processor
				:private typeconfig
			{
			public:

				typedef std::size_t size_t;

			public:

				processor();
				processor(const machine_ptr&);
				processor(const processor&) = delete;
				template<class ForwardIterator>
				processor(ForwardIterator begin, ForwardIterator end)
					:_mrunable(begin, end)
				{

				}

				template<class ForwardIterator>
				processor(const machine_ptr&, ForwardIterator begin, ForwardIterator end)
					: _mrunable(begin, end), processor(m)
				{

				}
				~processor();

				void		wait();
				void		wait_exit();
				void		exit();
				const	pstate state() const;
				const	machine_ptr& current_machine() const;
				void		addtask(srclist<shared_ptr<goroutine> >&, 
									goroutine_iterator, std::size_t count);
			private:

				lock_type		_mlock;
				shared_ptr<machine>	_mcurm;
				std::atomic<pstate>		_mstate;	
				srclist<goroutine_ptr>	_mrunable, _mfreed;
				
				void _state(const pstate);
				const size_t _alloc_half(processor&);
				void _init_(pstate state, const machine_ptr& curm);
				
				//~~~~~~~~~~~~~~~~~
				friend class phand_for_gosrc;
				friend class phand_for_g;
				friend class phand_for_sched;
			};
		}
	}
}
#endif