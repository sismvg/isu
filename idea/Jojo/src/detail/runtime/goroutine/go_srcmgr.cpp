
//File:go_srcmgr.cpp
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/runtime_mgr.h>
#include <go/runtime_tool.h>
#include <go/detail_handle.h>
#include <go/tools.h>

using namespace std;
namespace isu
{
	namespace go
	{
		namespace detail
		{

			go_srcmgr::go_srcmgr()
				:_malloc_id(0), _mgcount(0), _mfreeg_count(0)
			{

			}

			go_srcmgr::~go_srcmgr()
			{
			}

			shared_ptr<goroutine>
				go_srcmgr::gogo(functional f)
			{
				scoped_lock _lck(_mlock);
				return _allocg(f);
			}

			void go_srcmgr::task(functional f)
			{
				scoped_lock _lck(_mlock);
				_mrunableg.push_back( _makeg(f) );
				_mgcount.post();
			}

			const std::size_t go_srcmgr::runableg_size() const
			{//
#ifndef USE_BOOST_GOROUTINE
				return _malloc_id - _mfreeg_count;
#endif
			}

			const std::size_t go_srcmgr::freeg_size() const
			{//
				return _mfreeg_count;
			}

			shared_ptr<goroutine> go_srcmgr::_makeg(functional f)
			{
				return shared_ptr<goroutine>(new goroutine(_alloc_id(), f) );
			}

			go_srcmgr::goroutine_ptr
				go_srcmgr::_allocg(functional f)
			{
/*
#ifndef USE_BOOST_GOROUTINE
				//Fiber用Free没什么用
				goroutine_ptr g = _alloc_freeg(f);
				if (g) return g;
				g = _makeg(f);
				return g;*/
				return _makeg(f);
			}

			go_srcmgr::goroutine_ptr
				go_srcmgr::_alloc_freeg(functional f)
			{
				for (processor_ptr& proc : *global_processor_manager().get() )
				{
					if (phand_for_gosrc(proc).alloc_freeg(_mrunableg))
					{
						assert(!_mrunableg.empty());
						goroutine_ptr& ret = _mrunableg.back();
						ret->rebind(f);
						return ret;
					}
				}
				return goroutine_ptr();
			}

			const go_srcmgr::id_type go_srcmgr::_alloc_id()
			{
				return ++_malloc_id;
			}

		}
	}
}