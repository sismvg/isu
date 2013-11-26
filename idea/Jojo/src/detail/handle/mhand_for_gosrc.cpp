/*

#include <go/detail/handle/mhand_for_gosrc.h>
#include <go/detail/runtime/machine/machine.h>
#include <go/detail/runtime/processor/processor.h>
#include <go/detail/runtime/thread/thread.h>
#include <go/detail/runtime/goroutine/goroutine.h>
#include <iostream>
using namespace std;
namespace isu
{
	namespace go
	{
		namespace detail
		{
		/ *	typedef typeconfig::scoped_lock scoped_lock;

			mhand_for_gosrc::goroutine_ptr
				mhand_for_gosrc::switchg(const goroutine_ptr& g)
			{
				//暂时没有context能力
				
				return obj()->switchg(g);//没有绑定的时候就返回g
			}

			void mhand_for_gosrc::deatch()
			{
				obj()->_mthread->detach();
			}

			void mhand_for_gosrc::suppend()
			{
				obj()->_mthread->pause();
			}

			void mhand_for_gosrc::resume()
			{
				obj()->_mthread->resume();
			}

			const mhand_for_gosrc::context_type& mhand_for_gosrc::context()
			{
				return obj()->_mthread->get_context();
			}

			void mhand_for_gosrc::context(const context_type& con)
			{
				obj()->_mthread->set_context(con);
			}* /
		}
	}
}*/