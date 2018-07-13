/*
** Copyright (C) 2012 Fargier Sylvain <sfargier@wyplay.com>
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
** api.h
**
**        Created on: Apr 11, 2012
**   Original Author: sfargier
**
*/

#ifndef __API_H__
#define __API_H__

#if defined(__cplusplus)
#define BEGIN_DECLS extern "C" {
#define END_DECLS }
#define EXTERN_C extern "C"
#else
#define BEGIN_DECLS
#define END_DECLS
#define EXTERN_C
#endif

#undef __GNUC_REQ
#if defined(__GNUC__)
#define __GNUC_REQ(maj, min) ((__GNUC__ == maj && __GNUC_MINOR__ >= min) || __GNUC__ > maj)
#else
#define __GNUC_REQ(maj, min) 0
#endif

#if __GNUC_REQ(3, 3)
#define HIDDEN __attribute__ ((visibility("hidden")))
#define EXPORT __attribute__ ((visibility("default")))
#define NOTHROW __attribute__ ((__nothrow__))
#else
#define HIDDEN
#define EXPORT
#define NOTHROW
#endif

#if __GNUC_REQ(3, 2)
#define NOTHROW __attribute__ ((__nothrow__))
#else
#define NOTHROW
#endif

#if __GNUC_REQ(2, 96)
#define PURE __attribute__ ((__pure__))
#else
#define PURE
#endif

#if __GNUC_REQ(2, 5)
#define CONST __attribute__ ((__const__))
#else
#define CONST
#endif

#endif
