
#include <go/gohelper.h>
#include <go/runtime_mgr.h>
#include <go/runtime_tool.h>
#include <go/runtime_mgr.h>
#include <boost/thread/tss.hpp>
#include <go/detail/tools/mgrgeter.h>

//File:gohelper.cpp
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

namespace isu
{
	namespace go
	{
		namespace detail
		{
			extern boost::thread_specific_ptr<
							shared_ptr<detail::machine> > 
								afxcurrent_machine;

			extern boost::thread_specific_ptr<
				shared_ptr<detail::processor> >
								afxcurrent_processor;
		}

		namespace
		{
			typedef detail::typeconfig::processor_ptr processor_ptr;
			template<class Func>
			void _each_procmgr(Func f)
			{
				for (processor_ptr& p
					: *detail::global_processor_manager().get())
				{
					f(p);
				}
			}
		}

			detail::shared_ptr<detail::machine> curm_ptr()
			{
				auto ptr=detail::afxcurrent_machine.get();
				return ptr ? *ptr : detail::shared_ptr<detail::machine>();
			}

			detail::shared_ptr<detail::machine> current_machine()
			{
				return curm_ptr();
			}

			detail::shared_ptr<detail::processor> current_processor()//注意不是系统的processor
			{
				return curm_ptr()->curproc();
			}

			detail::shared_ptr<detail::goroutine> current_goroutine()
			{
				auto ptr = curm_ptr();
				return ptr ? ptr->curg(): detail::shared_ptr<detail::goroutine>();
			}

			//暂时没有办法实现全局wait..
			void		wait_exit()
			{
				_each_procmgr([](const processor_ptr& p)
				{
					p->wait_exit();
				});
			}

			void		stop_world()
			{

			}

			void		resume_world()
			{

			}

			const	bool at_goroutine()
			{
				return current_goroutine().get() != nullptr;
			}

			const	std::size_t max_processors()
			{
				return detail::global_processor_manager()->maxproc();
			}

			const	std::size_t processor_count()
			{
				return detail::global_processor_manager()->proc_size();
			}

			void		max_processors(const std::size_t size)
			{
				detail::global_processor_manager()->maxproc(size);
			}

			const	std::size_t syscall_machine()
			{
				return detail::global_machine_manager()->syscall();
			}

			const	std::size_t runing_machine()
			{
				return detail::global_machine_manager()->runing();
			}

			const	std::size_t idled_machine()
			{
				return detail::global_machine_manager()->idleing();
			}

			const	std::size_t runableg()
			{
				return detail::global_goroutine_manager()->runableg_size();
			}
		
			const	std::size_t	freeg()
			{
				return detail::global_goroutine_manager()->freeg_size();
			}

			const	std::size_t gid()
			{
				return current_goroutine()->id();
			}

			const	std::size_t mid()
			{
				return current_machine()->id();
			}

			void		yield()
			{

				auto g = current_goroutine();
				g->yield();
			}
	}
}