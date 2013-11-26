
#include <go/detail/handle/mhand_for_g.h>
#include <go/detail/runtime/machine/machine.h>
#include <go/detail/runtime/processor/processor.h>
#include <go/detail/runtime/thread/thread.h>
#include <go/detail/runtime/goroutine/goroutine.h>

//File:mhand_for_g.cpp
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
			typedef typeconfig::scoped_lock scoped_lock;

			mhand_for_g::goroutine_ptr
				mhand_for_g::switchg(const goroutine_ptr& g)
			{
				//暂时没有context能力
				return obj()->_switchg(g);//没有绑定的时候就返回g
			}

			void mhand_for_g::deatch()
			{
				obj()->_mthread->detach();
			}

			void mhand_for_g::setg(const goroutine_ptr& g)
			{
				obj()->_mcurg = g;
			}

			void mhand_for_g::suppend()
			{
				obj()->_mthread->pause();
			}

			void mhand_for_g::resume()
			{
				obj()->_mthread->resume();
			}
		}
	}
}