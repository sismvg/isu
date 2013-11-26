#ifndef ISU_GO_MACHINE_H
#define ISU_GO_MACHINE_H

//File:machine.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "mstate.h"
#include <boost/thread/tss.hpp>
#include <go/detail/config/typedef_config.h>
#include <go/detail/config/thread_forward.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class thread;	
			class machine;
			class goroutine;
			class processor;
			class mhand_for_g;
			class mhand_for_p;
			class mhand_for_sched;
			class mhand_for_sched;
	
		extern boost::thread_specific_ptr<
			shared_ptr<detail::machine> >
			current_machine;

		extern boost::thread_specific_ptr<
			shared_ptr<detail::processor> >
			current_processor;

			class machine
				:private typeconfig
			{
			public:
				typedef unsigned long id_type;

				machine(const id_type,const goroutine_ptr&, const processor_ptr&);
				machine(const machine&) = delete;
				~machine();

				void		exit();
				void		wait();
				void		wait_exit();
				const	mstate state() const;
				const	id_type id() const;
				void		graceful_exit();//和exit不一样，不是退出，而是设置退出模式
				const	goroutine_ptr& curg() const;
				const	processor_ptr& curproc() const;
				processor_ptr rebind( processor_ptr);
				
			private:
				lock_type			_mlock;
				thread*				_mthread;
				const id_type	_mid;

				std::atomic<mstate>		_mstate;
				shared_ptr<goroutine>	_mcurg;
				shared_ptr<processor>	_mcurp;

				
				void _start();
				void _state(const mstate);
				shared_ptr<goroutine> 
						_setg(const shared_ptr<goroutine>&);
				shared_ptr<goroutine>
						_switchg(const shared_ptr<goroutine>&);
				//~~~~~~~~~~~~~~~~~~~
				void _init_tss();
				static void _start_machine(machine* ptr);//嘛。。总不能为了调用线程而copy自己吧。
				friend class mhand_for_g;
				friend class mhand_for_p;
				friend class mhand_for_sched;
				friend class mhand_for_sched;
			};
		}
	}
}
#endif