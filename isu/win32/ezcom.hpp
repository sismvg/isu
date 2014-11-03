#pragma  once

#include <memory>

#include <isu/win32/string.hpp>
#include <isu/win32/lowercast_def.hpp>
#include <isu/win32/lowercast_com_def.hpp>

namespace isu
{
	namespace win32
	{
		template<class CastT,class T>
		CastT* interface_cast(T* object)
		{
			CastT* tmp = dynamic_cast<CastT*>(object);
			tmp->AddRef();
			return tmp;
		}

		template<class CastT,class T>
		std::shared_ptr<CastT> interface_cast(T* object, const iid& interface_id)
		{
			CastT* myface = nullptr;
			hresult result = 
				object->QueryInterface(interface_id, reinterpret_cast<void**>(&myface));
			if (FAILED(result))
				return nullptr;
			return std::shared_ptr<CastT>(myface, [](CastT* object)
			{
				object->Release();
			});
		}

		template<class CastT, class T>
		hresult interface_cast(T* object, void** ppvObject)
		{
			CastT* tmp = dynamic_cast<CastT*>(object);
			tmp->AddRef();
			*ppvObject = tmp;
			return S_OK;
		}

	}
}