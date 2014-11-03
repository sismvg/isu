#ifndef ISU_SHARED_HANDLE_HPP
#define ISU_SHARED_HANDLE_HPP

#include <memory>
#include <iostream>
#include <functional>

#include <windows.h>

#include <isu/win32/lowercast_def.hpp>

namespace isu
{
	namespace win32
	{
		//TODO:���ƹ��캯����Ҫд��
		//��������һ���ں˼�������
		//��Ӹ�native_handle
		//RES:�Ƿ���Ӷ�weak_ptr��֧�֣��Ͼ�reset�ᵼ��ԭ�е�deleter��Ч
#define SHARED_HANDLE_DELETOR(fn)[=](handle_type* ptr)\
		{ \
		if (ptr)\
		{\
		std::cout << "ɾ����GDI" << std::endl; \
		fn(*ptr); \
		delete ptr; \
	}\
	}
#define VOID_RESULT_FN(fn) [=](handle_type ptr){fn(ptr);}
#define SHARED_HANDLE_CONSTRUCT_IMPL(class_name,type,fn)\
	class_name(type handle)\
	:basic_shared_handle(handle, VOID_RESULT_FN(fn))\
		{}

#define SHARED_HANDLE_RESET_DEF(class_name,type,fn)\
		void reset(type handle)\
		{\
			basic_shared_handle::reset(handle, VOID_RESULT_FN(fn));\
		}
#define SHARED_HANDLE_CONSTRUCT(class_name,fn)\
	class_name(){}\
	SHARED_HANDLE_CONSTRUCT_IMPL(class_name,handle_type,fn)\
	SHARED_HANDLE_CONSTRUCT_IMPL(class_name,handle_type*,fn)\
	SHARED_HANDLE_RESET_DEF(class_name,handle_type,fn)\
	SHARED_HANDLE_RESET_DEF(class_name,handle_type*,fn)
		template<class T>
		class basic_shared_handle
			:public std::shared_ptr<T>
		{
		public:
			typedef T handle_type;
			typedef std::shared_ptr<T> father_type;
			typedef std::function<void(handle_type)> deletor;
			typedef size_t count_type;
			//����handle_type*��handle_typeһ����
			basic_shared_handle()
				:father_type(nullptr)
			{}
			basic_shared_handle(handle_type handle, deletor tor)
				:father_type(nullptr, SHARED_HANDLE_DELETOR(tor))
			{
				reset(handle,tor);
			}

			basic_shared_handle(handle_type* handle, deletor tor)
				:father_type(handle, SHARED_HANDLE_DELETOR(tor))
			{}
			void reset(handle_type handle,deletor tor)
			{
				auto* ptr = new handle_type(handle);
				reset(ptr, tor);
			}
			void reset(handle_type* handle,deletor tor)
			{
				father_type::reset(handle, SHARED_HANDLE_DELETOR(tor));
			}
			operator handle_type() const
			{
				return *get();
			}
		};
		class shared_handle
			:public basic_shared_handle<HANDLE>
		{
		public:
			SHARED_HANDLE_CONSTRUCT(shared_handle,CloseHandle)
		};

		template<class GDI>
		class shared_gdi
			:public basic_shared_handle<GDI>
		{
		public:
			SHARED_HANDLE_CONSTRUCT(shared_gdi,DeleteObject)
		};

		class shared_dc
			:public basic_shared_handle<HDC>
		{//��Ȼ���и�ReleaseDC,����ǰ���Ǳ�����hwnd�Ĵ���
		public:
			SHARED_HANDLE_CONSTRUCT(shared_dc, DeleteDC);
		};

		template<class GDI>
		shared_gdi<GDI> make_shared_gdi(GDI object)
		{
			return shared_gdi<GDI>(object);
		}

		shared_handle make_shared_handle(HANDLE ha);
	}
}
#endif