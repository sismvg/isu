#ifndef ISU_GO_GOROUTINE_H
#define ISU_GO_GOROUTINE_H

//File:goroutine.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <utility>
#include <go/detail/config/config.h>
#include <go/detail/config/typedef_config.h>
#include <go/detail/config/thread_forward.h>
#include <go/detail/handle/goroutine_handle.h>
#include <go/detail/runtime/goroutine/gstate.h>
#include <go/detail/runtime/goroutine/coroutine.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class goroutine_handle;

			class goroutine
			{
			public:			
				typedef std::size_t size_type;
				typedef std::size_t id_type;
				typedef typeconfig::function				function;
				typedef coroutine<function>			coroutine;	
				typedef std::function<function>		functional;
				typedef thread_forward::context_type context_type;	
				typedef typeconfig::lock_type			lock_type;
				typedef typeconfig::scoped_lock		scoped_lock;
			public:
				goroutine(const id_type, const functional&);
				const context_type& context();
				void context(const context_type&);
				void operator()();
				const bool empty() const;
				const id_type get_id() const;
				const gstate state() const;

				void rebind(const functional& f);
			private:
				lock_type _mlock;
				id_type _mid;	
				gstate _mstate;
				void _start();
				void _check_state(gstate);
				void _nextg();
				void _state(const gstate);
				coroutine			_mcoro;
				context_type	_mcontext;
				//~~~~~~~~~~~~~~~~~~~~~~~~
				friend class goroutine_handle;
			};
		}
	}
}
#endif