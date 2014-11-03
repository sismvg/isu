#pragma once

#include <Unknwn.h>

class timer_interface
	:public IUnknown
{
public:
	typedef void(*timer_callback)(time_t);
	virtual HRESULT __stdcall set_timer(timer_callback, long millisecond) = 0;
};