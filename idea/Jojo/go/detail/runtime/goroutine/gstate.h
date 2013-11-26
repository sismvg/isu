#ifndef ISU_GO_GSTATE_H
#define ISU_GO_GSTATE_H

//File:gstate.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

namespace isu
{
	namespace go
	{
		enum gstate
		{
			grunable, grunning, gdead, gidle, gsyscall,gswitching,gyield
		};
	}
}
#endif