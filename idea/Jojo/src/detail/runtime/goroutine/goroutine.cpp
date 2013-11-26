
//File:goroutine.cpp
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <thread>
#include <go/gohelper.h>
#include <go/go_except.h>
#include <go/runtime_mgr.h>
#include <go/detail/tools/mgrgeter.h>
#include <go/detail/handle/phand_for_g.h>
#include <go/detail/handle/mhand_for_g.h>
#include <go/detail/runtime/machine/machine.h>
#include <go/detail/runtime/goroutine/goroutine.h>


namespace isu
{
	namespace go
	{
		namespace detail
		{
			namespace
			{
				typedef ::isu::go::error_status<gstate> error_status;
			}
			
			goroutine::goroutine(const id_type id, const functional& f)
				: _mid(id), _mcoro(std::bind(_startg, this)), _mf(f)
			{
				_mstate.store(grunable);
			}

			goroutine::~goroutine()
			{
				if (!_mlock.try_lock())
				{
					_mlock.unlock();
				}
			}

			typedef void* lpvoid;
			boost::thread_specific_ptr<lpvoid> main_control;
	
			void _init_main_fiber()
			{
				if (main_control.get() == nullptr)
				{
					main_control.reset(new lpvoid(ConvertThreadToFiber(nullptr) ) );
				}
			}

			void goroutine::operator()()
			{
				_init_main_fiber();
				_mcoro();
				while (true)
				{//当前G必须处于死亡或者切换状态才能调用
					gstate st = state();
					assert(st == gdead || st == gswitching);
					_entruster();
				}
			}

			void goroutine::switchg(goroutine& g)
			{	
				if (g._mcoro.runed())
				{
					//如果g是已经初始化的那么必须要委托主控来切换
					//否则会出错
					_entrust_switch(*this, g);
					assert(false);
				}
				_mcoro.switchc_kill(g._mcoro);
			}

			void goroutine::wait_exit()
			{
				_state(gdead);
				wait();
			}

			void goroutine::wait()
			{
				_mlock.lock();
			}

			void goroutine::rebind(const functional& f)
			{
				if (_mstate == grunning || _mstate == gidle)
					throw error_status("goroutine can not rebind in running or idle",_mstate);
				_mf = f;
				_state(grunable);
			}

			const bool goroutine::empty() const
			{
				return _mcoro.empty();
			}

			const goroutine::id_type goroutine::id() const
			{
				return _mid;
			}

			const gstate goroutine::state() const
			{
				return _mstate;
			}

			void goroutine::_state(const gstate st)
			{
				_mstate = st;
			}

			void goroutine::yield()
			{
				//找到一个G
				phand_for_g		p = isu::go::current_processor();
				auto g = p.allocg_nowait();
				if (!g) return;
				//把自己放回runable队列
				yield_to(g);
			}

			void goroutine::yield_to(const shared_ptr<goroutine>& g)
			{
				phand_for_g		p = isu::go::current_processor();
				mhand_for_g	m = isu::go::current_machine();
				p.runableg(m.obj()->curg());
				m.setg(g);
				_state(gyield);
				_mcoro.yield_to(g->_mcoro);
				_state(grunning);
			}

			void goroutine::_start()
			{
				_mstate.store(grunning);
				_mf();
				_mstate.store(gswitching);
			}

			void goroutine::_check_state(gstate st)
			{
				if (_mstate!=st)
					throw error_status("Check state error",_mstate);
			}

			void goroutine::_nextg()
			{
				phand_for_g		p =	isu::go::current_processor();
				mhand_for_g	m	=	isu::go::current_machine();
				assert(m);
				if (p)
				{					  
					p.freeg( m.obj()->curg() );
					m.switchg(p.allocg());
				}
				_state(gdead);

				global_scheduler()->idle(m.obj());
				assert(false);//注意，绝对不会到这里来运行,因为在获取下一个G的时候绝对不会运行自己了
			}

			void goroutine::_startg(goroutine* g)
			{
				g->_check_state(grunable);
				g->_mlock.lock();
				g->_start();
				g->_mlock.unlock();
				if (g->_mstate == gdead) return;
				g->_nextg();//进入了以后会屏蔽调度
			}

			//windows Fiber专用
			void goroutine::_entrust_switch(goroutine& dest, goroutine& go)
			{
				typedef std::pair<lpvoid, lpvoid> arg;
				arg* temp = new arg(dest._mcoro.fiber(), go._mcoro.fiber() );
				SetFiberData(*main_control, temp);
				SwitchToFiber(*main_control);
			}

			void goroutine::_entruster()
			{//牵扯到内部实现
				lpvoid cur = GetCurrentFiber();
				typedef std::pair<lpvoid, lpvoid> arg;
				arg* data = reinterpret_cast<arg*>(GetFiberData(cur));
				assert(data&&data->first != cur);
				DeleteFiber(data->first);
				lpvoid temp = data->second;
				delete data;//效率瓶颈
				SwitchToFiber(temp);
			}
		}
	}
}