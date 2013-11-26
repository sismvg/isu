#ifndef ISU_GO_EXCEPT_H
#define ISU_GO_EXCEPT_H

//File:go_except.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <stdexcept>

namespace isu
{
	namespace go
	{
		template<class T>
		class error_status
			:public std::runtime_error
		{
		public:	

			typedef T state_type;

			error_status(const char* msg, const state_type st)
				:_mstate(st), std::runtime_error(msg)
			{}

			const state_type& state() const
			{
				return _mstate;
			}

		private:
			const state_type _mstate;
		};
	}
}
#endif