/*
** Copyright (C) 2010 Fargier Sylvain <fargier.sylvain@free.fr>
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
** chrono.hh
**
**        Created on: Sep 24, 2010
**            Author: fargie_s
**
*/

#ifndef __CHRONO_HH__
#define __CHRONO_HH__

#include <ostream>
#include <string>
#include <time.h>
#include <stdint.h>

#if defined(__MACH__)
#include <mach/mach_time.h>
#endif

class Chrono
{
public:
  Chrono();

  void clear();
#if defined(__MACH__) /* OSX support */
  inline void start() { m_times[0] = mach_absolute_time(); }
  inline void stop() { m_times[1] = mach_absolute_time(); }
#else
  inline void start() { clock_gettime(CLOCK_MONOTONIC, &m_times[0]); }
  inline void stop() { clock_gettime(CLOCK_MONOTONIC, &m_times[1]); }
#endif

  uint32_t getMiliSecs() const;
  operator std::string() const;
  void getTimespec(struct timespec &timespec) const;
  Chrono &operator += (const Chrono &);

protected:
#if defined __MACH__
  uint64_t m_times[2]; /* in absolute unit */
  mach_timebase_info_data_t m_info;
#else
  struct timespec m_times[2];
#endif
};

std::ostream &operator<< (std::ostream &os, const Chrono &chrono);

#endif /* __CHRONO_H__ */

