#include <go/detail/runtime/thread/thread.h>

//File:thread.cpp
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
			thread::context_type thread::get_context()
			{
				CONTEXT con;
				GetThreadContext(native_handle(), &con);
				return con;
			}
			void thread::set_context(const context_type& con)
			{
				SetThreadContext(native_handle(), &con);
			}

			void thread::pause()
			{
				SuspendThread(this->native_handle() );
			}
			void thread::resume()
			{
				ResumeThread(this->native_handle() );
			}
		}
	}
}