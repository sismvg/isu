#ifndef ISU_GO_DETAIL_HANDLE_H
#define ISU_GO_DETAIL_HANDLE_H

//File:detail_handle.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#include <go/detail/handle/phand_for_g.h>
#include <go/detail/handle/mhand_for_p.h>
#include <go/detail/config/typedef_config.h>
#include <go/detail/handle/phand_for_gosrc.h>
#include <go/detail/handle/phand_for_sched.h>
#include <go/detail/handle/ghand_for_sched.h>
#include <go/detail/handle/mhand_for_sched.h>
#include <go/detail/handle/gosrc_mhand_for_p.h>
#include <go/detail/handle/p_mgrhand_for_sched.h>
#include <go/detail/handle/m_mgrhand_for_sched.h>

typedef isu::go::detail::typeconfig::goroutine_ptr goroutine_handle;

#endif