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
** fileLock.cc
**
**        Created on: Sat Feb  5 15:09:59 2011
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cassert>

#include "fileLock.hh"

FileLock::FileLock(int fd) :
  m_fd(fd), m_mode(FL_UNLOCK)
{
}

FileLock::FileLock(const FileLock &) {
  assert(0 && "Can't copy a FileLock");
}

FileLock::~FileLock()
{
  if (m_fd >= 0 && m_mode != FL_UNLOCK)
    unlock();
}

int FileLock::lock(mode m, bool wait)
{
  struct flock f;
  memset(&f, 0, sizeof (struct flock));

  f.l_whence = SEEK_SET;
  switch (m) {
    case FL_WR:
      f.l_type = F_WRLCK;
      break;
    case FL_RD:
      f.l_type = F_RDLCK;
      break;
    case FL_UNLOCK:
      f.l_type = F_UNLCK;
      return EINVAL;
  }

  int rc;
  if (wait)
    rc = fcntl(m_fd, F_SETLKW, &f);
  else
    rc = fcntl(m_fd, F_SETLK, &f);

  if (rc == 0) {
    m_mode = m;
    return 0;
  }
  else
  	return errno;
}

int FileLock::unlock()
{
  struct flock f;
  memset(&f, 0, sizeof (struct flock));

  f.l_whence = SEEK_SET;
  f.l_type = F_UNLCK;

  int rc = fcntl(m_fd, F_SETLK, &f);
  if (rc == 0) {
    m_mode = FL_UNLOCK;
    return 0;
  }
  else
  	return errno;
}

