#ifndef ISU_GO_GOHELPER_H
#define ISU_GO_GOHELPER_H

//File:gohelper.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/config/typedef_config.h>
#include <go/detail/runtime/machine/mstate.h>
#include <go/detail/runtime/goroutine/gstate.h>

namespace isu
{
	namespace go
	{
		namespace detail
		{
			class machine;
			class processor;
			class goroutine;
		}
		//其实这里的函数都可以用global....函数来获取对象，然后使用
		//这里不过是少点代码
		void		wait_exit();
	//void		stop_world();
	//	void		resume_world();
		const	bool at_goroutine();
		const	std::size_t max_processors();
		const	std::size_t processor_count();
		void		max_processors(const std::size_t);
		const	std::size_t freeg();
		const	std::size_t runableg();
		const	std::size_t idled_machine();
		const	std::size_t syscall_machine();
		const	std::size_t runing_machine();
		detail::shared_ptr<detail::goroutine>	current_goroutine();//G不能对自己进行修改

		//必须处于G才可以使用的
		detail::shared_ptr<detail::machine>	current_machine();
		detail::shared_ptr<detail::processor>	current_processor();//注意不是系统的processor
		const	std::size_t gid();
		const	std::size_t mid();
		void		yield();
	}
}

#endif