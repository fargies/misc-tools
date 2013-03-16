/*
** Copyright (C) 2012 Sylvain Fargier <sylvain.fargier@somfy.com>
**
** This software is provided 'as-is', without any express or implied
** warranty.  In no event will the authors be held liable for any damages
** arising from the use of this software.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software. If you use this software
**    in a product, an acknowledgment in the product documentation would be
**    appreciated but is not required.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
** utils.h
**
**        Created on: Aug 04, 2012
**   Original Author: Sylvain Fargier <fargier.sylvain@free.fr>
**
*/

#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef XSTR
#define XSTR(s) STR(s)
#define STR(s) #s
#endif

#ifndef UNUSED
#if defined(__GNUC__)
#define UNUSED(a) a __attribute__ ((unused))
#else
#define UNUSED(a) a
#endif

#if defined(__cplusplus)

#ifndef NDEBUG

/* From: Modern C++ Design */
template<int> struct CompileTimeChecker;
template<> struct CompileTimeChecker<true> {};
#define STATIC_CHECK(expr, msg) \
{ \
        CompileTimeChecker<((expr) != 0)> ERROR_##msg; \
        (void)ERROR_##msg; \
}

#else

#define STATIC_CHECK(expr, msg) do { } while(0)

#endif

#endif

#endif

