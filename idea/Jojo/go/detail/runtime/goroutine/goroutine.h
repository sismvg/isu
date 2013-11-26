#ifndef ISU_GO_GOROUTINE_H
#define ISU_GO_GOROUTINE_H

//File:goroutine.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include "gstate.h"
#include <utility>
#include <go/detail/config.h>
#include <go/detail/runtime/goroutine/coroutine.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class gscheder;//为调度器分配的句柄
			
			class goroutine
				:private typeconfig
			{
			public:			
				typedef std::size_t size_type;
				typedef std::size_t id_type;
				typedef typeconfig::function					function;
				typedef coroutine<function>				coroutine;	
				typedef boost::function<function>		functional;
				typedef thread_forward::context_type	context_type;	
			public:

				goroutine(const id_type, const functional&);
				goroutine(const goroutine&) = delete;
				~goroutine();

				void					operator()();
				const bool		empty() const;
				const id_type	id() const;
				const gstate		state() const;
				void	yield();
				void	wait();
				void	wait_exit();
				void switchg(goroutine&);//切换运行，会删除调用自己的fiber
				void rebind(const functional& f);
				void	yield_to(const shared_ptr<goroutine>&);
			private:
				lock_type	_mlock;
				const id_type			_mid;
				std::atomic<gstate>	_mstate;
				coroutine			_mcoro;
				context_type	_mcontext;
				functional			_mf;
				void _start();
				void _check_state(gstate);
				void _nextg();
				void _state(const gstate);
				static void _entrust_switch(goroutine&, goroutine&);
				static void _entruster();
				static void _startg(goroutine*);
				//~~~~~~~~~~~~~~~~~~~~~~~~
				friend class gscheder;
			};
		}
	}
}
#endif