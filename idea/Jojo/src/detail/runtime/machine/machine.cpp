
//File:machine.cpp
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/runtime_tool.h>
#include <go/detail/runtime/thread/thread.h>



namespace isu
{
	namespace go
	{
		namespace detail
		{
			boost::thread_specific_ptr<
				shared_ptr<detail::machine> >
				afxcurrent_machine;

			boost::thread_specific_ptr<
				shared_ptr<detail::processor> >
				afxcurrent_processor;

			machine::machine(
				const id_type id,
				const goroutine_ptr& start,
				const processor_ptr& p )
				: _mcurp(p), _mcurg(start), _mid(id)
			{
				if (start&&p)
				_mstate.store(mrunable);
			}

			machine::~machine()
			{
				if (_mthread != nullptr)
					_mthread->interrupt();//析构的时候必须中断
			}

			const mstate machine::state() const
			{
				return _mstate;
			}

			const shared_ptr<processor>&  machine::curproc() const
			{
				return _mcurp;
			}

			machine::processor_ptr machine::rebind(processor_ptr ptr)
			{
				std::swap(_mcurp, ptr);
				return ptr;
			}

			void machine::wait_exit()
			{
				_mcurg->wait_exit();
				exit();
			}

			void machine::graceful_exit()
			{
				wait_exit();
			}
			
			void machine::wait()
			{
				_mcurg->wait();
			}

			void machine::exit()
			{
				if (_mthread)
					_mthread->interrupt();
			}

			const  shared_ptr<goroutine>&  machine::curg() const
			{
				return _mcurg;
			}

			const machine::id_type machine::id() const
			{
				return _mid;
			}

			shared_ptr<goroutine> machine::_setg(const shared_ptr<goroutine>& g)
			{
				scoped_lock _lck(_mlock);
				auto ret = _mcurg;
				_mcurg = g;
				return ret;
			}

			shared_ptr<goroutine> machine::_switchg(const shared_ptr<goroutine>& g)
			{
				auto result = _setg(g);
				if (result&&g)
				{
				//	(*g)();//栈溢出问题
					result->switchg(*g.get());
				}
				_state(g ? mrunable : midle);
				return result;
			}

			void machine::_state(const mstate st)
			{
				_mstate = st;
			}

			void machine::_init_tss()
			{
				afxcurrent_processor.reset(new processor_ptr(_mcurp));
				afxcurrent_machine.reset(new machine_ptr(_mcurp->current_machine()));
			}

			void machine::_start_machine(machine* ptr)
			{
				assert(ptr);
				ptr->_init_tss();
				__try
				{
					(*ptr->_mcurg)();
					assert(false);//不会运行到这里
				}
				__except (EXCEPTION_STACK_OVERFLOW)
				{
					return;
				}
			}
		}
	}
}