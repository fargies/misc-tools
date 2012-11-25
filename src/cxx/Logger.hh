/*
** Copyright (C) 2012 Fargier Sylvain <fargier.sylvain@free.fr>
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
** Logger.hh
**
**        Created on: Nov 25, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __LOGGER_HH__
#define __LOGGER_HH__

#include <stdio.h>

#define log_error(...) do { printf(__VA_ARGS__); printf("\n"); } while (0);
#define log_warning(...) do { printf(__VA_ARGS__); printf("\n"); } while (0);
#define log_notice(...) do { printf(__VA_ARGS__); printf("\n"); } while (0);
#define log_info(...) do { printf(__VA_ARGS__); printf("\n"); } while (0);
#define log_debug(...) do { printf(__VA_ARGS__); printf("\n"); } while (0);

namespace log {


void log(int priority, const char *format, ...);

void debug(const char *);

}

#endif

