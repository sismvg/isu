
#include "treeview.hpp"
namespace isu
{
	namespace gui
	{
		void draw_grid(hwnd wnd, hdc dc, std::size_t item_count,int height_interval)
		{
			rect_type rect;
			GetClientRect(wnd, &rect);
			int total_height = item_count*height_interval;
			MoveToEx(dc, rect.left, rect.top, NULL);
			LineTo(dc, rect.left, rect.top+total_height);

			MoveToEx(dc, rect.right, rect.top, NULL);
			LineTo(dc, rect.right, rect.top + total_height);
			//横线
			point_type pt = { rect.left, rect.top };
			point_type end_pt = { rect.right, rect.top };
			++item_count;
			while (item_count--)
			{
				MoveToEx(dc, pt.x, pt.y, NULL);
				LineTo(dc, end_pt.x, end_pt.y);
				pt.y += height_interval;
				end_pt.y += height_interval;
			}
		}

		treeview_ui_impl::treeview_info::treeview_info()
			:self(nullptr), oldproc(nullptr)
		{}
		//treeview_ui_impl直接dynamic_cast到下面就好了
		treeview_ui_impl::treeview_info::treeview_info(treeview_ui_impl* obj, wndproc proc)
			:self(obj), oldproc(proc)
		{}
		treeview_ui_impl::treeview_ui_impl(hwnd wnd,size_type style)
			:_wnd(wnd), _style(style)
		{}
		hwnd treeview_ui_impl::native_handle() const
		{
			return _wnd;
		}
		hwnd treeview_ui_impl::replace_wnd(hwnd wnd)
		{
			hwnd result = _wnd;
			_wnd = wnd;
			return result;
		}
		void treeview_ui_impl::srcoll_state(scroll_state)
		{}
		void treeview_ui_impl::style(size_type sty)
		{
			_style = sty;
		}
		treeview_ui_impl::size_type treeview_ui_impl::style() const
		{
			return _style;
		}
		void treeview_ui_impl::lock_ui()
		{
			style(_style | gui_locked);
		}
		void treeview_ui_impl::unlock_ui()
		{
			style(_style | gui_unlocked);
		}
		bool treeview_ui_impl::gui_is_locked() const
		{
			return _style&gui_locked;
		}

		//detail
		void treeview_ui_impl::_update_scrollinfo(hwnd wnd,dword bar,size_type item_count)
		{
			scrollinfo info;
			info.cbSize = sizeof(info);
			info.fMask = SIF_ALL;
			GetScrollInfo(wnd, bar, &info);
			double rate_in_pos = 0;
			if (info.nMax != 0)
				rate_in_pos = static_cast<double>(info.nPos) / (info.nMax-info.nPage+1);
			info.nMin = 0;
			if (bar == SB_VERT)
			{
				info.nMax = _avg_item_height*(item_count - 1);
				info.nPage = _items_of_page*_avg_item_height;
			}
			else if (bar == SB_HORZ)
			{
				rect_type rect;
				GetClientRect(wnd, &rect);
				info.nPage = rect.right - rect.left;
				info.nMax = 2 * info.nPage;
			}
			info.nPos = info.nMax*rate_in_pos;
			info.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
			SetScrollInfo(wnd, bar, &info, TRUE);
		}
		void treeview_ui_impl::_init_memory_dc(hwnd wnd)
		{
			if (wnd == NULL)
				return;
			hdc dc = GetDC(wnd);
			hdc mem_dc = CreateCompatibleDC(dc);
			rect_type rect;
			GetClientRect(wnd, &rect);
			int height = (rect.bottom += _buffer_items*_avg_item_height) - rect.top;
			int weight = rect.right - rect.left;
			HBITMAP bitmap = CreateCompatibleBitmap(dc, weight, height);
			SelectObject(mem_dc, bitmap);
			FillRect(mem_dc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
			SetBkMode(mem_dc, TRANSPARENT);
			_memory_dc.replace_dc(mem_dc);
			ReleaseDC(native_handle(), dc);
			_x_trans = 0; _y_trans = 0;
		}
		scrollinfo treeview_ui_impl::_adjustment_scroll(size_type style,dword bar, wparam warg, lparam larg)
		{
			scrollinfo info;
			info.cbSize = sizeof(info);
			info.fMask = SIF_ALL;
			GetScrollInfo(native_handle(), bar, &info);
			long prepos = info.nPos;
			dword word = LOWORD(warg);
			if (word == SB_TOP || word == SB_LEFT)
				info.nPos = info.nMin;
			else if (word == SB_BOTTOM || word == SB_RIGHT)
				info.nPos = info.nMax;
			else if (word == SB_LINEUP || word == SB_LINELEFT)
				info.nPos = max(info.nPos - larg, info.nMin);
			else if (word == SB_LINEDOWN || word == SB_LINEUP)
				info.nPos = min(info.nPos + larg, info.nMax);
			else if (word == SB_PAGEUP || word == SB_PAGELEFT)
				--info.nPage;
			else if (word == SB_PAGEDOWN || word == SB_PAGERIGHT)
				++info.nPage;
			else if (word == SB_THUMBTRACK)
				info.nPos = info.nTrackPos;
			SetScrollInfo(native_handle(), bar, &info, TRUE);
			info.nTrackPos = prepos;
			return info;
		}
		void treeview_ui_impl::_adjustment_show_trans()
		{
			scrollinfo info;
			info.cbSize = sizeof(info);
			info.fMask = SIF_ALL;
			GetScrollInfo(_wnd, SB_VERT, &info);
			if (info.nPos == (info.nMax - info.nPage) + 1)
				_y_trans = _avg_item_height;
			else if (_avg_item_height != 0)
				_y_trans = info.nPos % _avg_item_height;
			GetScrollInfo(_wnd, SB_HORZ, &info);
			_x_trans = info.nPos;
		}
		void treeview_ui_impl::_clear_memory_dc(hdc dc,int height,int weight)
		{
			height += _buffer_items*_avg_item_height;
			rect_type rect(0, 0, weight, height);
			FillRect(_memory_dc.native_handle(), &rect, CreateSolidBrush(RGB(255, 255, 255)));
		}
		void treeview_ui_impl::_next_position(size_type index, int& x, int& y)
		{
			y += _avg_item_height;
		}
		void treeview_ui_impl::_reset_size(int height, int weight)
		{
			long items_of_page = _items_of_page;
			_avg_item_height = height / items_of_page;
			_init_memory_dc(native_handle());
		}
		void treeview_ui_impl::_init_wnd(hwnd prewnd, hwnd wnd)
		{
			auto& map = _view_map();
			long old_wndproc = GetWindowLong(wnd, GWL_WNDPROC);
			auto iter = map.find(prewnd);
			if (iter != map.end())
			{
				SetWindowLong(prewnd, GWL_WNDPROC,
					reinterpret_cast<long>(iter->second.oldproc));
				map.erase(iter);
			}
			wndproc newproc = reinterpret_cast<wndproc>(old_wndproc);
			map.insert(std::make_pair(wnd, treeview_info(this, newproc)));
			rect_type rect;
			GetClientRect(wnd, &rect);
			_reset_size(rect.height(), rect.weight());
		}
	}
}