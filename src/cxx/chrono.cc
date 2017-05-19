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
** chrono.cc
**
**        Created on: Sep 24, 2010
**            Author: fargie_s
**
*/

#include <iomanip>
#include <sstream>
#include <string.h>

#include "chrono.hh"

Chrono::Chrono() {
  clear();
}

void Chrono::clear() {
#if defined(__MACH__)
  mach_timebase_info(&m_info);
#endif
  memset(m_times, 0, sizeof (m_times));
}

uint32_t Chrono::getMiliSecs() const {
#if defined(__MACH__)
    return (m_times[1] - m_times[0]) / 1000000;
#else
  struct timespec ts;
  getTimespec(ts);

  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}

Chrono::operator std::string() const {
  std::ostringstream oss;
  struct timespec ts;
  getTimespec(ts);

  oss.fill('0');
  oss << ts.tv_sec << "." << std::setw(9) << ts.tv_nsec;
  return oss.str();
}

Chrono &Chrono::operator += (const Chrono &c) {
#if defined(__MACH__)
  m_times[1] += c.m_times[1] - c.m_times[0];
  return *this;
#else
  getTimespec(m_times[0]); // getTimespec must work on itself
  c.getTimespec(m_times[1]);

  uint32_t i = m_times[0].tv_nsec + m_times[1].tv_nsec;
  m_times[1].tv_nsec = i % 1000000000;
  m_times[1].tv_sec += m_times[0].tv_sec + ((i >= 1000000000) ? 1 : 0);

  memset(m_times, 0, sizeof(struct timespec));
  return *this;
#endif
}

std::ostream &operator << (std::ostream &os, const Chrono &chrono) {
  return os << chrono.operator std::string();
}

void Chrono::getTimespec(struct timespec &out) const {
#if defined(__MACH__)
  uint64_t delta = (m_times[1] - m_times[0]) * m_info.numer / m_info.denom;
  out.tv_sec = delta / 1000000000;
  out.tv_nsec = delta % 1000000000;
#else
  if (m_times[1].tv_nsec < m_times[0].tv_nsec) {
    out.tv_sec = m_times[1].tv_sec - m_times[0].tv_sec - 1;
    out.tv_nsec = 1000000000 + m_times[1].tv_nsec - m_times[0].tv_nsec;
  }
  else {
    out.tv_sec = m_times[1].tv_sec - m_times[0].tv_sec;
    out.tv_nsec = m_times[1].tv_nsec - m_times[0].tv_nsec;
  }
#endif
}
