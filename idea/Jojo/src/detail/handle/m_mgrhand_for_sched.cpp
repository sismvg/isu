
#include <go/detail/machine.h>
#include <go/detail/handle/mhand_for_sched.h>
#include <go/detail/handle/m_mgrhand_for_sched.h>

//File:m_mgrhand_for_sched.cpp
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
			namespace
			{
				template<class Iter,class Con>
				void hashset_iter_assert(const Iter& iter, const Con& c)
				{
					assert(iter != c.end());
				}

				template<class Con,class Id_type>
				void mov(Con& lhs, Con& rhs, const Id_type id)
				{
					auto iter = lhs.find(id);
					hashset_iter_assert(iter, lhs);
					rhs.insert(*iter);
					lhs.erase(iter);
				}
			}

			void m_mgrhand_for_sched::idle_to_runing(const id_type id)
			{
				_mov(obj()->_midle, obj()->_mrunning, id);
			}

			void m_mgrhand_for_sched::syscall_to_runing(const id_type id)
			{
				_mov(obj()->_msyscall, obj()->_mrunning, id);
			}

			void m_mgrhand_for_sched::idle(const id_type id)
			{
				_mov(obj()->_mrunning, obj()->_midle, id);
			}

			void m_mgrhand_for_sched::syscall(const id_type id)
			{
				_mov(obj()->_mrunning, obj()->_msyscall, id);
			}

			void m_mgrhand_for_sched::_mov(map_type& lhs, map_type& rhs, const id_type id)
			{
				scoped_lock _lck(obj()->_mlock);
				mov(lhs, rhs, id);
			}
			
				m_mgrhand_for_sched::machine_ptr
				m_mgrhand_for_sched::allocm(const goroutine_ptr& g,
				const processor_ptr& proc_iter)
			{
				scoped_lock _lck(obj()->_mlock);
				if (!obj()->_midle.empty())
				{
					auto m_iter = obj()->_midle.begin();
					shared_ptr<machine>& m = m_iter->second;
					mhand_for_sched handle(m);
					handle.rebind(proc_iter);
					handle.switchg(g);
					auto ret=obj()->_mrunning.insert(*m_iter);
					obj()->_midle.erase(m_iter);
					return m;
				}
				id_type id = _alloc_m_id();
				auto ret=
					obj()->_mrunning.insert(
					std::make_pair(id, 
						machine_ptr(new machine(id,g, proc_iter ) ) ) );
				assert(ret.second);
				return ret.first->second;
			}

			const m_mgrhand_for_sched::id_type 
				m_mgrhand_for_sched::_alloc_m_id()
			{
				static std::atomic<id_type> sid(0);
				return ++sid;
			}
		}
	}
}