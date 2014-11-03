
#pragma once


#define PROXY_FN(id,name,result) template<class... Arg>\
	result name(Arg... args)\
	{\
	this->invoke<id>(args...); \
	}

#define INVOKE_IMPL  dispatch->Invoke(\
	disp_id, IID_NULL, LOCALE_USER_DEFAULT, \
	DISPATCH_METHOD, &param, &result, NULL, NULL)
namespace isu
{
	namespace win32
	{
		HRESULT dispatch_invoke_impl(unsigned disp_id,IDispatch* dispatch)
		{
			CComVariant result;
			CComVariant* invoke_arg = nullptr;
			DISPPARAMS param = { NULL, NULL, 0, 0 };
			return INVOKE_IMPL;
		}

		template<class... Args>
		HRESULT dispatch_invoke_impl
			(unsigned long disp_id,IDispatch* dispatch, Args... args)
		{
			CComVariant result;
			CComVariant invoke_arg[] =
			{
				args...
			};
			DISPPARAMS param = { NULL, NULL, 0, 0 };
			param.rgvarg = invoke_arg;
			param.cArgs = sizeof...(args);
			return INVOKE_IMPL;
		}

		template<class T, class... Args>
		HRESULT dispatch_invoke(unsigned long disp_id,T* connection_point_impl, Args... args)
		{
			HRESULT hr = S_OK;
			int connection_count = connection_point_impl->m_vec.GetSize();
			for (int index = 0; index != connection_count; ++index)
			{
				connection_point_impl->Lock();
				CComPtr<IUnknown> punkConnection =
					connection_point_impl->m_vec.GetAt(index);
				connection_point_impl->Unlock();
				IDispatch * dispatch = static_cast<IDispatch *>(punkConnection.p);
				if (dispatch)
				{
					return dispatch_invoke_impl(disp_id, dispatch, args...);
				}
			}
			return hr;
		}

		template<class T, class ConnectionPointImpl>
		class basic_event_proxy
			:public ConnectionPointImpl
		{
		public:
			template<unsigned long DispID, class... Arg>
			HRESULT invoke(Arg... args)
			{
				return dispatch_invoke(DispID,static_cast<T*>(this), args...);
			}

			template<class... Arg>
			HRESULT invoke_dynamic(unsigned long disp_id, Arg... args)
			{
				return dispatch_invoke(disp_id, static_cast<T*>(this), args...);
			}
		};
	}
}