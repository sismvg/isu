#ifndef ISU_GO_CONFIG_H
#define ISU_GO_CONFIG_H

//File:config.h
// (C) Copyright 2013 四元飒
// Use, modification and distribution is subject to the Isu source code
// License, Version 0.1. (See local file LICENSE_0_1.txt)
// See http://blog.csdn.net/fq_hayate for updates, documentation, and revision history.
//Q&A:a313865974@sina.com

#define USE_BOOST
#define USE_WIN_FIBER
#define USE_BOOST_THREAD

//USE_WIN_FIBER 使用Fiber
//USE_BOOST_GOROUTINE 使用boost.coroutine
//USE_BOOST_THREAD 使用boost.thread
//USE_WIN_THREAD 使用windows的线程(包装而已)
//USE_STL_THREAD 使用std::thread 需要编译器支持Cxx11
//USE_BOOST 使用boost(暂时必须要用)
#endif