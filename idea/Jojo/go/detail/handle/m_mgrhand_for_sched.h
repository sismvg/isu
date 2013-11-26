#ifndef ISU_GO_HANDLE_M_MGRHAND_FOR_SCHED_H
#define ISU_GO_HANDLE_M_MGRHAND_FOR_SCHED_H

//File:m_mgrhand_for_sched.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <hash_map>
#include "access_handle.h"

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class machine_mgr;
			class scheduler;

			ACCESS_HANDLE(m_mgrhand_for_sched,machine_mgr,scheduler)
		public:
			typedef unsigned long id_type;
			void idle_to_runing(const id_type);
			void syscall_to_runing(const id_type);
			void idle(const id_type);
			void syscall(const id_type);
			machine_ptr allocm(const goroutine_ptr& g, const processor_ptr&);
		private:
			typedef unsigned long id_type;
			const id_type _alloc_m_id();
		private:
			typedef std::hash_map<id_type,shared_ptr<machine> > map_type;
			void _mov(map_type&, map_type&,const id_type);
			ACCESS_HANDLE_END
		}
	}
}
#endif