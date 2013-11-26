#ifndef ISU_GO_SRCMGR_H
#define ISU_GO_SRCMGR_H

//File:go_srcmgr.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/config.h>
#include <boost/function.hpp>
#include <go/detail/tools/srclist.h>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class go_srcmgr_handle;
			class  gosrc_mhand_for_p;
			//专业G管理
			class go_srcmgr
				:private typeconfig
			{
			public:
				typedef boost::function<function> functional;		
				typedef std::size_t size_t;
				typedef std::size_t id_type;
			public:
				go_srcmgr();
				~go_srcmgr();
				
				const std::size_t runableg_size() const;
				const std::size_t freeg_size() const;
				shared_ptr<goroutine>
						gogo(functional);//不会保存g
				void task(functional);
			private:
				lock_type						_mlock;
				boost::interprocess::interprocess_semaphore
													_mgcount;
				std::atomic<size_t>		_malloc_id;
				std::atomic<size_t>		_mfreeg_count;
				srclist<goroutine_ptr>	_mrunableg;

				void					_gogo(functional);
				void					_send_task();
				goroutine_ptr	_makeg(functional);
				goroutine_ptr	_allocg(functional);
				goroutine_ptr	_alloc_freeg(functional);
				const id_type	_alloc_id();
				
				//~~~~~~~~~~~~~~~~~~~~
				friend class go_srcmgr_handle;
				friend class gosrc_mhand_for_p;
			};
		}
	}
}
#endif