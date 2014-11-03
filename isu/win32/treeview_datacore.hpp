#ifndef ISU_TREEVIEW_DATACORE_HPP
#define ISU_TREEVIEW_DATACORE_HPP

#include <vector>
#include <iterator>
#include <memory>
#include <algorithm>
#include <functional>
#include <hash_map>

#include <isu/alg/vector_alg.hpp>

#include "listbox_detail_marco.hpp"

namespace isu
{
	namespace gui
	{
		template<class Storage>
		class mytreeview
			:private Storage
		{
		public:
			CON_DEF(Storage);
			typedef Storage storage_core;
			typedef mytreeview self_type;

			iterator begin()
			{
				return DIS_FN(begin)();
			}
			iterator end()
			{
				return DIS_FN(end)();
			}
			const_iterator cbegin() const
			{
				return DIS_FN(cbegin)();
			}
			const_iterator cend() const
			{
				return DIS_FN(cend)();
			}
			const_iterator begin() const
			{
				return DIS_FN(begin)();
			}
			const_iterator end() const
			{
				return DIS_FN(end)();
			}
			size_type size() const
			{
				return DIS_FN(size)();
			}
			bool empty() const
			{
				return DIS_FN(empty)();
			}
			void push_back(const value_type& value)
			{
				return DIS_FN(push_back)(value);
			}
			storage_core* container()
			{
				return static_cast<storage_core*>(this);
			}
			const storage_core* container() const
			{
				return static_cast<const storage_core*>(this);
			}
		};
		//data
		//push_back,insert,erase,erase_node,trun_node
		//data only list->push_front
		//size,empty,node_size,node_empty,is_node,get_node
		//ui
		//expand_all,expand,narrow,narrow_all
		//for extent
		//alloc_data_range,data_of_range
		//each_listbox_as
		template<class Storage>
		class treeview_random_access
			:public mytreeview<Storage>
		{
		public:
			//construct
			typedef treeview_random_access<storage_core> self_type;
			typedef std::shared_ptr<self_type> view_pointer;
			typedef std::pair<size_type, size_type> data_range;
			class data_view
			{
			public:
				data_view()
				{}
				data_view(self_type* core, size_type begin, size_type end)
					:_data_core(core), _begin(begin), _end(end)
				{
					
				}
				~data_view()
				{}
				size_type size() const
				{
					return end() - begin();
				}
				bool empty() const
				{
					return size() == 0;
				}
				const_iterator begin() const
				{
					return _data_core->_data_iter_of(_begin);
				}
				const_iterator end() const
				{
					return _data_core->_data_iter_of(_end);
				}
				void trans_range(long length)
				{//保证区间大小
					long advanced = length > 0 ? right_extent(length) : left_extent(length);
					long next_advance = (std::min)(std::abs(length), advanced);
					length > 0 ? left_extent(next_advance) : right_extent(-next_advance);
				}
				long left_extent(long length)
				{
					return _extent_impl(_begin, length);
				}
				long right_extent(long length)
				{
					return _extent_impl(_end, length);
				}
			private:
				long _extent_impl(size_type& position, long length)
				{
					long tmp = position;
					tmp += length;
					_data_core->_adj_size(tmp);
					long result = tmp - position;
					position = tmp;
					return std::abs(result);
				}
				self_type* _data_core;
				size_type _begin;
				size_type _end;
			};
			treeview_random_access()
			{}
			treeview_random_access(const storage_core& core)
				:father_type(core)
			{}
			treeview_random_access(const value_type& value, size_type count)
				:father_type(value, count)
			{}
			template<class ForwardIterator>
			treeview_random_access(ForwardIterator begin, ForwardIterator end)
				: father_type(begin, end)
			{}
			//generic member
			iterator insert(iterator where, const value_type& value)
			{
				iterator result_iter = container()->insert(where, value);
				_node_change(result_iter, 1);
				return result_iter;
			}
			void erase(iterator where)
			{
				erase_node(where);
				size_type pos = _covt_index(where);
				container()->erase(where);
				_node_change(container()->begin() +pos, -1);
			}
			view_pointer trun_node(iterator where)
			{
				return trun_node(where, self_type());
			}
			view_pointer trun_node(iterator where, const self_type& core)
			{
				size_type abs_index = _covt_index(where);
				auto node_iter = isu::alg::near_binary_search(_subviews, abs_index);
				sublistbox_position position
					(abs_index, view_pointer(new self_type(core)));
				_subviews.insert(node_iter, position);
				return position.list;
			}
			view_pointer erase_node(iterator where)
			{
				auto iter = _bsearch_view(where);
				view_pointer result(nullptr);
				if (iter != _subviews.end())
				{
					result = iter->list;
					_subviews.erase(iter);
				}
				return result;
			}
			view_pointer get_node(iterator where)
			{
				auto iter = _bsearch_view(item_iter);
				return iter == _subviews.end() ?
					view_pointer(nullptr) : iter->list
			}
			bool is_node(iterator where)
			{
				return find_sublist(where) != nullptr;
			}
			//extent member
			data_view alloc_data_range(size_type begin_pos, size_type end_pos)
			{
				return data_view(this, begin_pos, end_pos);
			}
			template<class Iterator,class Func>
			void each_view_as(Iterator begin_iter, Iterator end_iter, Func fn)
			{
				auto bind_fn = std::bind(fn, std::placeholders::_1, false, nullptr);
				if (_subviews.empty())
				{
					std::for_each(begin_iter, end_iter, bind_fn);
				}
				else
				{
					size_type last = 0;
					std::for_each(_subviews.begin(), _subviews.end(),
					[&](const  sublistbox_position& position)
					{
						std::for_each(begin() + last, begin() + position.index, bind_fn);
						fn(*(begin() + position.index), true, position.list);
						last = position.index + 1;
					});
					std::for_each(begin() + last, end(), bind_fn);
				}
			}
		private:
			const_iterator _data_iter_of(long pos)
			{
				_adj_size(pos);
				return begin() + pos;
			}
			std::pair<const_iterator,const_iterator>
				_alloc_data_range(size_type begin_pos, size_type end_pos)
			{
					_adj_size(begin_pos); _adj_size(end_pos);
					return std::make_pair(begin() + begin_pos, begin() + end_pos);
			}
			void _node_change(iterator where, int count)
			{
				size_type abs_index = _covt_index(where);
				if (abs_index == size())
					return;
				auto node_iter = isu::alg::near_binary_search(_subviews, abs_index);
				std::for_each(node_iter, _subviews.end(), 
				[&](sublistbox_position& position)
				{
					position.index += count;
				});
			}
			void _adj_size(long& position)
			{
				long size_ = static_cast<long>(size());
				position = position >= size_ ? size_ : (std::max)(position, static_cast<long>(0));
			}
			size_type _covt_index(iterator where) const
			{
				return where - begin();
			}
			struct sublistbox_position
			{
				typedef sublistbox_position self_type;
				sublistbox_position(size_type pos,view_pointer pointer)
					:index(pos), list(pointer)
				{}
				operator size_type() const
				{
					return index;
				}
				size_type index;
				view_pointer list;
			};
			//总是排序的
			std::vector<sublistbox_position> _subviews;
			typename std::vector<sublistbox_position>::const_iterator
				_bsearch_view(iterator item_iter) const
			{
					size_type pos = _covt_index(item_iter);
					auto iter = std::lower_bound(_subviews.begin(), _subviews.end(), pos);
					if (iter != _subviews.end())
					{
						return pos == iter->index ? iter : _subviews.end();
					}
					return iter;
				}
		};

		template<class Storage>
		class data_view
		{
		public:
			CON_DEF(Storage)
			typedef Storage storage_core;
			typedef storage_core self_type;
			data_view()
			{}
			data_view(self_type* core, size_type begin, size_type end)
				:_data_core(core)
			{
				_range = _data_core->_alloc_data_range(begin, end);
			}
			~data_view()
			{
				_data_core->_release_range(*_range);
			}
			const_iterator begin() const
			{
				return _range->first;
			}
			const_iterator end() const
			{
				return _range->second;
			}
			size_type size() const
			{
				auto iter = _range->first;
				size_type result = 0;
				while (iter != _range->second)
				{
					++result;
					++iter;
				}
				return result;
			}
			bool empty() const
			{
				return size() == 0;
			}
			void trans_range(long length)
			{//保证区间大小不变
				long advanced = length > 0 ? right_extent(length) : left_extent(length);
				long next_advance = (std::min)(std::abs(length), advanced);
				length > 0 ? left_extent(next_advance) : right_extent(-next_advance);
			}
			
			long right_extent(long length)//不保证begin<end
			{
				return _advance(_range->second, length);
			}
			long left_extent(long length)//不保证begin<end
			{
				return _advance(_range->first, length);
			}
		private:
			long _advance_impl(
				const_iterator& iter, const_iterator boundary, 
				size_type length, bool forward)
			{
				size_type index = 0;
				for (; index != length&&iter!=boundary; ++index)
				{
					forward ? ++iter : --iter;
				}
				return index;
			}
			long _advance(const_iterator& iter, long length)
			{
				const_iterator boundary =
					length < 0 ? _data_core->begin() : _data_core->end();
				//length小于0的话只能到begin,大于0的话只能到end
				return _advance_impl(iter, boundary, std::abs(length), length > 0);
			}
			self_type* _data_core;
			std::shared_ptr<std::pair<const_iterator, const_iterator>>
				_range;
		};
		template<class Storage>
		class treeview_bid_access
			:public mytreeview<Storage>
		{//嘛，的确有点太长了
		public:
			//construct
			typedef treeview_bid_access<storage_core> self_type;
			typedef std::shared_ptr<self_type> view_pointer;
			typedef std::pair<const_iterator,const_iterator> data_range;
			typedef data_view<self_type> data_view;
			friend class data_view;
			treeview_bid_access()
			{}
			treeview_bid_access(const storage_core& core)
				:father_type(core)
			{}
			treeview_bid_access(const value_type& value, size_type count)
				:father_type(value, count)
			{}
			template<class ForwardIterator>
			treeview_bid_access(ForwardIterator begin, ForwardIterator end)
				: father_type(begin, end)
			{};
			//generic member
			iterator insert(iterator where, const value_type& value)
			{
				return container()->insert(where, value);
			}
			void erase(iterator where)
			{
				erase_node(where);
				value_type* ptr = std::addressof(*where);
				auto iter = _data_adjer.find(ptr);
				if (iter != _data_adjer.end())
				{
					_adj_iter(*(iter->second));
					if (*iter->second != end())
					{
						const_iterator* new_iter = iter->second;
						_data_adjer.erase(iter);
						_data_adjer.insert(std::make_pair(ptr, new_iter));
					}
					else
					{
						_data_adjer.erase(iter);
					}
				}
				DIS_FN(erase)(where);
			}
			view_pointer trun_node(iterator where)
			{
				return trun_node(where, self_type());
			}
			view_pointer trun_node(iterator where, const self_type& node)
			{
				view_pointer node_ptr(new self_type(node));
				_subviews.insert(std::make_pair(std::addressof(*where), node_ptr));
				return node_ptr;
			}
			view_pointer erase_node(iterator where)
			{
				auto node_iterator = _subviews.find(std::addressof(*where));
				view_pointer result(nullptr);
				if (node_iterator != _subviews.end())
				{
					result = node_iterator->second;
					_subviews.erase(node_iterator);
				}
				return result;
			}
			view_pointer get_node(iterator where)
			{
				auto iter = _subviews.find(std::addressof(*where));
				return 
					iter == _subviews.end() ? view_pointer(nullptr) : iter->second;
			}
			bool is_node(iterator where)
			{
				return get_node(where) != nullptr;
			}
			//extent member
			data_view alloc_data_range(size_type begin_pos, size_type end_pos)
			{
				return data_view(this, begin_pos, end_pos);
			}
			template<class Iterator, class Func>
			void each_view_as(Iterator begin_iter, Iterator end_iter, Func fn)
			{
				std::for_each(begin_iter,end_iter, [&](const value_type& value)
				{
					auto iter = _subviews.find(const_cast<value_type*>(std::addressof(value)));
					iter == _subviews.end() ? 
						fn(value, false, nullptr) : fn(value, true, iter->second);
				});
			}
		private:
			std::shared_ptr<std::pair<const_iterator,const_iterator>>
				_alloc_data_range(size_type begin_pos,size_type end_pos)
			{
				typedef std::pair<const_iterator, const_iterator> range;
				if (begin_pos >= size() || end_pos > size())
					return std::shared_ptr<range>(new range(end(), end()));
				std::shared_ptr<range> result
					(new range(abs_pos_iter(begin_pos), abs_pos_iter(end_pos)));
				_insert(result->first); _insert(result->second);
				return result;
			}

			void _release_range(data_range& range)
			{
				_release_iter(range.first);
				_release_iter(range.second);
			}
			void _release_iter(const_iterator data_iter)
			{
				if (data_iter == end()) return;
				auto iter = _data_adjer.find(const_cast<value_type*>(std::addressof(*data_iter)));
				if (iter != _data_adjer.end())
					_data_adjer.erase(iter);
			}
			void _insert(const_iterator& iter)
			{
				if (iter != end())
				{
					_data_adjer.insert(std::make_pair(std::addressof(*iter), &iter));
				}
			}
			iterator abs_pos_iter(size_type pos)
			{
				iterator end_iter = begin();
				if (pos != -1)
					std::advance(end_iter, pos);
				else
					end_iter = end();
				return end_iter;
			}
			void _adj_iter(const_iterator& iter)
			{
				//三种情况都要处理..
				empty() ? iter = begin() : ++iter;
			}
			typedef std::hash_map<value_type*, view_pointer> listboxs_map;
			listboxs_map _subviews;
			std::hash_map<const value_type*, const_iterator*> _data_adjer;
		};

		template<class T>
		struct treeview_base_is
		{
			typedef typename
			std::_If <
			std::is_same<
			typename std::iterator_traits<typename T::iterator>::iterator_category,
			std::random_access_iterator_tag>::value,
			treeview_random_access<T>, treeview_bid_access < T >> ::type type;
		};

		template<class Storage>
		class treeview_datacore
			:public treeview_base_is<Storage>::type
		{
		public:
			typedef treeview_datacore self_type;
			typedef typename treeview_base_is<Storage>::type father_type;
		};
	}
}
#endif