/*
** Copyright (C) 2011 Fargier Sylvain <fargier.sylvain@free.fr>
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
** rwLock.cc
**
**        Created on: Sat Feb  5 17:29:53 2011
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cassert>

#include "rwLock.hh"

RWLock::RWLock(const std::string &name) :
  m_name(name), m_mode(RWL_UNLOCK)
{
  m_rd = sem_open(("/" + name + "-RWLock-rd").c_str(), O_CREAT, 0666, 0);
  m_wr = sem_open(("/" + name + "-RWLock-wr").c_str(), O_CREAT, 0666, 1);
}

RWLock::~RWLock() {
  unlock();

  if (m_rd)
    sem_close(m_rd);
  if (m_wr)
    sem_close(m_wr);

  sem_unlink(("/" + m_name + "-RWLock-rd").c_str());
  sem_unlink(("/" + m_name + "-RWLock-wr").c_str());
}

int RWLock::lock(mode lock_mode, bool wait) {
  if (m_mode == lock_mode)
    return 0;

  if (!m_rd || !m_wr)
    return EINVAL;

  if (m_mode != RWL_UNLOCK)
    unlock();

  int rc = 0;
  int readers = 0;
  switch (lock_mode) {
    case RWL_READ:
      rc = sem_wait(m_wr);
      if (rc != 0)
        return rc;
      sem_post(m_rd);
      sem_post(m_wr);
      m_mode = RWL_READ;
      break;

    case RWL_WRITE:
      rc = sem_wait(m_wr);
      if (rc != 0)
        break;
      do {
        if (sem_getvalue(m_rd, &readers) != -1)
          if (readers == 0)
            break;
        usleep(1);
      } while (true);
      m_mode = RWL_WRITE;
      break;

    case RWL_UNLOCK:
      break;
  }

  return rc;
}

int RWLock::unlock() {
  if (!m_rd || !m_wr)
    return EINVAL;

  switch (m_mode) {
    case RWL_READ:
      sem_wait(m_rd);
      break;
    case RWL_WRITE:
      sem_post(m_wr);
      break;
    default:
      break;
  }
  m_mode = RWL_UNLOCK;
  return 0;
}

