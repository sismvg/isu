#ifndef ISU_TREEVIEW_HPP
#define ISU_TREEVIEW_HPP

#include <sstream>
#include <memory>
#include <functional>
#include <hash_map>

#include <windows.h>
#include <isu/win32/lowercast_def.hpp>
#include <isu/win32/dc.hpp>
#include "listbox_detail_marco.hpp"

namespace isu
{
	namespace gui
	{
		void draw_grid(hwnd wnd, hdc dc, std::size_t item_count, int height_interval);
		class treeview_ui_impl
		{
		public:
			typedef std::size_t size_type;
			static const size_type gui_locked = 0x0001, gui_unlocked = 0x0010;
			//其他需要自己控制
			treeview_ui_impl(hwnd wnd, size_type style);
			hwnd native_handle() const;
			hwnd replace_wnd(hwnd wnd);
			enum scroll_state{};
			void srcoll_state(scroll_state);
			void style(size_type sty);
			size_type style() const;
			void lock_ui();
			void unlock_ui();
			bool gui_is_locked() const;
		protected:
			hwnd _wnd;
			isu::gui::dc _memory_dc;
			size_type _items_of_page, _buffer_items;
			size_type _avg_item_height, _avg_item_weight;
			size_type _style;
			long _x_trans, _y_trans;
			//gui相关
			void _update_scrollinfo(hwnd, dword, size_type);
			void _init_memory_dc(hwnd);
			void _adjustment_show_trans();
			scrollinfo _adjustment_scroll(size_type style, dword bar, wparam warg, lparam larg);
			void _clear_memory_dc(hdc normal_dc, int height, int weight);
			void _next_position(size_type index, int& x, int& y);
			void _reset_size(int height, int weight);
			void _init_wnd(hwnd prewnd, hwnd wnd);
			//给windows用的
			struct treeview_info
			{
				treeview_info();
				//treeview_ui_impl直接dynamic_cast到下面就好了
				treeview_info(treeview_ui_impl* obj, wndproc proc);
				treeview_ui_impl* self;
				wndproc oldproc;
			};
			static std::hash_map<hwnd, treeview_info>& _view_map()
			{
				static std::hash_map<hwnd, treeview_info> _view_map_impl;
				return _view_map_impl;
			}
		};
		
		template<class TreeView>
		class treeview
			:public treeview_ui_impl
		{
		public:
			CON_DEF(TreeView)
			typedef TreeView treeview_datacore;
			typedef std::shared_ptr<treeview_datacore> data_core_ptr;
			typedef std::pair<size_type, size_type> data_range;
			typedef typename treeview_datacore::data_view data_view;
			typedef
				std::function<void(size_type, hdc, const rect_type&, const value_type&)> draw_item_proc;
			//wnd为NULL的时候必须手动创建个wnd
			//当range为nullptr的时候从0~items_of_page
			static void normal_proc(
				size_type index, hdc dc, const rect_type& rect, const value_type& value)
			{
				std::basic_stringstream<TCHAR> stream;
				stream << value;
				auto str = stream.str();
				TextOut(dc, rect.left, rect.top, str.c_str(), str.size());
			}
			treeview(
				data_core_ptr core, size_type items_of_page,draw_item_proc proc,
				size_type style=gui_unlocked,const data_range* range = nullptr, hwnd wnd = NULL)
				:treeview_ui_impl(wnd,style), _data_core(core)
			{
				_items_of_page = items_of_page;
				_buffer_items = 1;
				set_data_range(range ? *range : std::make_pair(0, _items_of_page));
				_update_gui_detail(native_handle(), wnd);
				_draw_item = proc;
			}
			~treeview()
			{
			}
			draw_item_proc replace_draw_item(draw_item_proc proc)
			{
				auto result = proc;
				_draw_item = proc;
				return result;
			}
			hwnd replace_wnd(hwnd wnd)
			{
				hwnd pre = _wnd;
				hwnd result = treeview_ui_impl::replace_wnd(wnd);
				_update_gui_detail(pre, wnd);
				return result;
			}
			void force_update(size_type style)
			{
				update_data();
				hdc dc = GetDC(native_handle());
				_draw_in(style, dc, nullptr);
				ReleaseDC(native_handle(), dc);
			}
			void update_data()
			{
				_update_scrollinfo(native_handle(), SB_VERT, _data_core->size());
				size_type item_count = _items_of_page + _buffer_items;
				if (_view.size()<item_count)
					set_data_range(_range);
				InvalidateRect(native_handle(), NULL, FALSE);
			}
			void data_core(data_core_ptr ptr)
			{
				_data_core = ptr;
				update_data();
			}
			data_core_ptr data_core() const
			{
				return _data_core;
			}
			void set_data_range(const data_range& range)
			{
				_view = _data_core->alloc_data_range(range.first, range.second + _buffer_items);
				_range = range;
			}
			const data_range& get_data_range() const
			{//编译错误
				auto iter = _data_core->cbegin();
				return std::make_pair
					(std::distance(iter, _view.begin()),std::distance(iter, _view.end() ) );
			}
			//画到指定的DC上
			void draw_in(hdc dc, paintstruct* paint)
			{
				_draw_in(dc, paint);
			}
		private:
			data_core_ptr _data_core;
			//绘画缓冲条目
			data_view _view;
			data_range _range;
			draw_item_proc _draw_item;
			void _draw_in(size_type style,hdc dc, paintstruct* paint)
			{
				rect_type rect;
				GetClientRect(native_handle(), &rect);
				if (style&gui_unlocked)
				{
					size_type index = 0;
					int x = 0, y = 0;
					_clear_memory_dc(NULL, rect.height(), rect.weight());
					std::for_each(_view.begin(), _view.end(), [&](const value_type& value)
					{
						rect_type item_rect(x, y, x + rect.weight(), y + _avg_item_height);
						_draw_item(index++, _memory_dc.native_handle(), item_rect, value);
						_next_position(index, x, y);
					});
					draw_grid(native_handle(), _memory_dc.native_handle(), index, _avg_item_height);
				}
				_adjustment_show_trans();
				BitBlt(
					dc, 0, 0,
					rect.weight(), rect.height(), _memory_dc.native_handle(), _x_trans, _y_trans, SRCCOPY);
			}
			void _update_gui_detail(hwnd prewnd,hwnd wnd)
			{
				update_data();
				_init_wnd(prewnd, wnd);
				_init_memory_dc(wnd);
				_x_trans = 0; _y_trans = 0;
			}
			void _init_wnd(hwnd prewnd, hwnd wnd)
			{
				treeview_ui_impl::_init_wnd(prewnd, wnd);
				SetWindowLong(wnd, GWL_WNDPROC, reinterpret_cast<long>(_treeview_proc));
			}
			bool _is_boundary(long prepos, long pos)
			{
				return
					(_data_core->end() == _view.end() && pos - prepos > 0)
					||
					(_data_core->begin() == _view.begin() && pos - prepos < 0);
			}

			void _adjustment_data_range(
				size_type style,dword scrolltype, uint page, long prepos, long pos)
			{
				if (style&gui_locked) return;
				long height = _avg_item_height;
				prepos /= height;pos /= height;
				if (scrolltype == SB_VERT)
				{
					if (!_is_boundary(prepos,pos))
					{//list的时候有点速度问题
					//	long to_end_length = std::distance(_view.end(), _data_core->cend());
						long length = pos - prepos;// (std::min)(to_end_length, pos - prepos);
						if (length)
							_view.trans_range(length);
					}
				}
			}
			void _adjustment_scroll(size_type style,dword bar,wparam warg,lparam larg)
			{//larg是一次滚动的长度
				if (style & gui_locked) return;
				scrollinfo info = treeview_ui_impl::_adjustment_scroll(style, bar, warg, larg);
				//nTrackPos其实是更新前的Pos，这样写节省点代码
				_adjustment_data_range(style, bar, info.nPage, info.nTrackPos, info.nPos);
			}

			static int _treeview_proc(hwnd wnd, uint message, wparam warg, lparam larg)
			{
				auto iter = _view_map().find(wnd);
				if (iter == _view_map().end()) return -1;
				//嘛，细节，细节
				treeview* self = reinterpret_cast<treeview*>(iter->second.self);
				wndproc oldproc = iter->second.oldproc;
				switch (message)
				{
				case WM_SIZE:
					self->_reset_size(HIWORD(larg), LOWORD(larg));
					self->force_update(gui_unlocked);
					break;
				case WM_VSCROLL:
					self->_adjustment_scroll(self->_style, SB_VERT, warg, larg);
					InvalidateRect(wnd, NULL, FALSE);
					break;
				case WM_HSCROLL:
					self->_adjustment_scroll(self->_style, SB_HORZ, warg, larg);
					InvalidateRect(wnd, NULL, FALSE);
					break;
				case WM_MOUSEWHEEL:
				{
					long detla = warg;
					SendMessage(
					wnd, WM_VSCROLL, detla > 0 ? SB_LINEUP : SB_LINEDOWN, WHEEL_DELTA);
				}
				  break;
					break;
				case WM_PAINT:
				{
					paintstruct paint;
					hdc dc = BeginPaint(wnd, &paint);
					self->_draw_in(self->_style, dc, NULL);
					EndPaint(wnd, &paint);
				}
					break;
				default:
					break;
				}
				if (message == WM_VSCROLL || message == WM_HSCROLL)
				{
					InvalidateRect(wnd, NULL, FALSE);
					SendMessage(wnd, WM_PAINT, 0, 0);
				}
				return oldproc(wnd, message, warg, larg);
			}
		};
	}
}
#endif