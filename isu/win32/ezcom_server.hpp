
#pragma once

#include <atomic>
#include <memory>

#include <isu/win32/string.hpp>
#include <isu/alg/more_for_each.hpp>
#include <isu/win32/lowercast_def.hpp>
#include <isu/win32/lowercast_com_def.hpp>

namespace isu
{
	namespace win32
	{
		class compent_server
		{
		public:
			typedef unsigned long count_type;
			static bool empty_example();
			static count_type add_example();
			static count_type release_example();
		private:
			static std::atomic<count_type> _example_count;
		};

		template<class UnknownBase>
		class ezunknown
			:public UnknownBase
		{
		public:
			dword COMCALL AddRef()
			{
				if (_ref_count == 0)
				{
					compent_server::add_example();
				}
				++_ref_count;
				return _ref_count;
			}

			dword COMCALL Release()
			{
				if (--_ref_count == 0)
				{
					compent_server::release_example();
					release_object(this);
					return 0;
				}
				return _ref_count;
			}
		private:
			std::atomic<unsigned long> _ref_count;
		};

		template<class T, class... Arg>
		T* create_object(Arg... arg)
		{
			return new T(arg...);
		}

		template<class T>
		void release_object(T* object)
		{
			delete object;
		}
		class class_factory_data;

		hresult register_local_factory(class_factory_data&);
		hresult unregister_local_factory(class_factory_data&);

		class class_factory_data
		{
		public:

			friend hresult register_local_factory(class_factory_data&);
			friend hresult unregister_local_factory(class_factory_data&);

			class_factory_data(const clsid&, interface_factory*);
			~class_factory_data();
			interface_factory* factory() const;
			dword cookie() const;

		private:
			clsid _factory_clsid;
			dword _magic_cookie;
			std::shared_ptr<interface_factory> _factory;
		};

		template<class ForwardIterator>
		hresult register_factory_table(ForwardIterator begin, ForwardIterator end)
		{
			hresult result = S_OK;
			isu::alg::for_each_if(begin, end, [&](class_factory_data& data)
				->bool
			{
				return SUCCEEDED((result = register_local_factory(data)));
			});
			return result;
		}

		template<class ForwardIterator>
		hresult unregister_factory_table(ForwardIterator begin, ForwardIterator end)
		{
			hresult result = S_OK;
			isu::alg::for_each_if(begin, end, [&](class_factory_data& data)
				->bool
			{
				return SUCCEEDED((result = unregister_local_factory(data)));
			});
			return result;
		}

		hresult register_server(
			const clsid& classid, const string& compent_path, 
			const string& progid,const string& description, const string& version);

		hresult unregister_server(const clsid& classid, const string& progid, const string& version);
	}
}