#ifndef ISU_ACCESS_HANDLE_H
#define ISU_ACCESS_HANDLE_H

//File:access_handle.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/config/typedef_config.h>

namespace isu
{
	template<class T,class Sign>
	class access_handle
	{
	public:

		typedef Sign dest_type;
		typedef access_handle this_type;
		typedef go::detail::shared_ptr<T> pointer;
		
		access_handle(const this_type& rhs)
			:_mT(rhs._mT)
		{

		}

		operator bool() const
		{
			return obj() != nullptr;
		}		

		access_handle(const pointer& obj)//必须在引用时初始化，否则无意义
			:_mT(obj)
		{

		}
		access_handle()
		{

		}
	protected:
		pointer obj()
		{
			return _mT;
		}

		const pointer& obj() const
		{
				return _mT;
		}


	private:
		pointer _mT;
	};
}

#define ACCESS_HANDLE(name,base1,base2)\
class name\
	:public access_handle<base1, base2>, private typeconfig\
{\
public:\
	typedef access_handle<base1, base2> father_type; \
	name(){}\
	name(const shared_ptr<base1>& t) :\
	father_type(t)\
{	}\
	friend class base1; \
	friend class base2;
#define ACCESS_HANDLE_END };
#endif