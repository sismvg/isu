
#include <go/tools.h>
#include <go/detail/runtime/machine/machine.h>
#include <go/detail/handle/gosrc_mhand_for_p.h>
#include <go/detail/runtime/processor/processor.h>

//File:processor.cpp
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
			processor::processor()
			{
				_init_(prunable, _mcurm);
			}

			processor::processor(const machine_ptr& m)
			{
				_init_(prunable, _mcurm);
			}

			processor::~processor()
			{
				scoped_lock _lck(_mlock);
				_mstate.store(pdead);
				if (!_mrunable.empty())
				{
					gosrc_mhand_for_p(global_goroutine_manager()).
						reciperare(_mrunable);
				}
			}

			const shared_ptr<machine>& 
						processor::current_machine() const
			{
				return _mcurm;
			}

			const pstate processor::state() const
			{
				return _mstate;
			}

			void processor::wait()
			{
				_mcurm->wait();
			}

			void processor::exit()
			{
				_mcurm->exit();
			}

			void processor::wait_exit()
			{
				_mcurm->wait_exit();
			}

			const processor::size_t processor::_alloc_half(processor& p)
			{
				std::size_t size = _mrunable.size();
				if (size==0) return 0;
				p._mrunable.splice(
					p._mrunable.end(), _mrunable,
					_mrunable.begin(), 
					advance(_mrunable.begin(),(size >= 2 ? size / 2 : 1) ) );
				return size;
			}

			void processor::addtask(
				srclist<shared_ptr<goroutine> >& list, 
					goroutine_iterator begin, std::size_t count)
			{
				_mrunable.splice( _mrunable.begin(), list,
						begin, isu::go::detail::advance(begin, count) );
			}

			void processor::_init_(pstate state, const machine_ptr& curm)
			{
				_mstate.store(state);
				_mcurm = curm;
			}
		}
	}
}