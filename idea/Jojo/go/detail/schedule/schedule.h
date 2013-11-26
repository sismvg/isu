#ifndef ISU_GO_DETAIL_SCHEDULE_H
#define ISU_GO_DETAIL_SCHEDULE_H

//File:schedule.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail_handle.h>
#include <go/detail/config/typedef_config.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class thread;

			class scheduler
				:private typeconfig
			{
			public:

				typedef unsigned long time_type;
				typedef boost::function<function> functional;
				typedef typeconfig::processor_iterator processor_iterator;

				scheduler();
				scheduler(const scheduler&) = delete;
				void interrupt(); 
				void idle(const shared_ptr<machine>&);
				void gogo(functional);
				void send_task();
				void entrust_free(functional);//负责machine不能做的事情

			private:

				void _send_task();
			//	thread* _mschedr;

				m_mgrhand_for_sched		_mmgrhand;
				p_mgrhand_for_sched		_mpmhand;
				std::vector<functional>	_mentrust;
				
		/*		static void _start();
				static processor_iterator end();
				static processor_iterator begin();
				static void timer_wait(time_type);
				static void _nextg(processor_ptr&);*/
			};
		}
	}
}
#endif