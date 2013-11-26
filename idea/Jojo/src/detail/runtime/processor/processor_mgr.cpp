
#include <go/detail/processor.h>
#include <go/detail/tools/utility.h>

//File:processor_mgr.cpp
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

			processor_mgr::processor_mgr(const size_t maxproc)
				: _mmaxproc(maxproc), _mnowproc(0)
			{
				_mmaxproc.store(maxproc);
			}

			void processor_mgr::interrupt()
			{
				scoped_lock _lck(_mlock);
				for (processor_ptr& proc : _mrunning)
					proc->exit();
			}

			const std::size_t processor_mgr::maxproc() const
			{
				return _mmaxproc;
			}

			const std::size_t processor_mgr::proc_size() const
			{
				return _mnowproc;
			}

			void		processor_mgr::maxproc(const std::size_t size)
			{
				scoped_lock _lck(_mlock);
				_mmaxproc = size;
				int kill_count = _mnowproc - _mmaxproc;
				assert(_mnowproc == _mrunning.size() );
				while(kill_count > 0)
				{
					auto iter = _mrunning.begin();
					(*iter)->exit();
					_mrunning.erase(iter);
					++iter;
				}
			}

			processor_mgr::processor_iterator 
						processor_mgr::begin()
			{
				return _mrunning.begin();
			}

			processor_mgr::processor_iterator
						processor_mgr::end()
			{
				return _mrunning.end();
			}
		}
	}
}