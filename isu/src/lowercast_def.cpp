
#include <windows.h>
#include <isu/win32/lowercast_def.hpp>

rect_type::rect_type()
{
	right = 0;
	left = 0;
	top = 0;
	bottom = 0;
}

rect_type::rect_type(int lft, int tp, int rgt, int btm)
{
	right = rgt; top = tp;
	left = lft; bottom = btm;
}

int rect_type::weight() const
{
	return right - left;
}
int rect_type::height() const
{
	return bottom - top;
}