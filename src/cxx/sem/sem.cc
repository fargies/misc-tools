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
** sem.cc
**
**        Created on: Feb 5, 2011
**            Author: fargie_s
**
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <cassert>
#include "sem.hh"

Sem::Sem(int pshared, unsigned int value)
{
  if (pshared >= 0) {
    m_sem = (sem_t *) malloc(sizeof (sem_t));
    sem_init(m_sem, pshared, value);
  } else
    m_sem = NULL;
}

Sem::~Sem()
{
  if (m_sem) {
    sem_destroy(m_sem);
    free(m_sem);
  }
}

int Sem::wait()
{
  if (sem_wait(m_sem) != 0)
    return errno;
  else
    return 0;
}

int Sem::trywait()
{
  if (sem_trywait(m_sem) != 0)
    return errno;
  else
    return 0;
}

int Sem::timedwait(unsigned int msecs)
{
  struct timespec ts;
  assert(clock_gettime(CLOCK_REALTIME, &ts) == 0);

  ts.tv_sec += msecs / 1000;
  ts.tv_nsec += (msecs % 1000) * 1000000;
  if (sem_timedwait(m_sem, &ts) != 0)
    return errno;
  else
    return 0;
}

int Sem::post()
{
  if (sem_post(m_sem) != 0)
    return errno;
  else
    return 0;
}

int Sem::getValue()
{
  int value = 0;

  sem_getvalue(m_sem, &value);
  return value;
}


NamedSem::NamedSem(const std::string &name, unsigned int value) :
  Sem(-1), m_name(name), m_unlink(false)
{
  m_sem = sem_open(name.c_str(), O_CREAT | O_RDWR, 0666, value);
}

NamedSem::~NamedSem() {
  sem_close(m_sem);

  if (m_unlink)
    sem_unlink(m_name.c_str());

  m_sem = NULL;
}

const std::string &NamedSem::getName() {
  return m_name;
}

void NamedSem::unlink() {
  m_unlink = true;
}

int NamedSem::unlink(const std::string &name) {
  if (sem_unlink(name.c_str()) != 0)
    return errno;
  else
    return 0;
}
