
#pragma once

#include <windef.h>

typedef HDC hdc;
typedef HWND hwnd;
typedef HANDLE handle;
typedef UINT uint;
typedef DWORD_PTR dword_ptr;
typedef DWORD dword;
typedef ULONG ulong;
typedef WPARAM wparam;
typedef LPARAM lparam;
typedef HRESULT hresult;
typedef HINSTANCE hinstance;
typedef SCROLLINFO scrollinfo;
typedef POINT point_type;
typedef PAINTSTRUCT paintstruct;
typedef WNDPROC wndproc;

struct rect_type
	:public RECT
{
	rect_type();
	rect_type(int lft, int tp, int rgt, int btm);
	int weight() const;
	int height() const;
};