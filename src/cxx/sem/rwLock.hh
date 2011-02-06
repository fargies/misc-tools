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
** rwLock.hh
**
**        Created on: Sat Feb  5 17:54:23 2011
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <string>
#include <semaphore.h>

class RWLock {
public:
  RWLock(const std::string &name);
  ~RWLock();

  typedef enum {
    RWL_READ,
    RWL_SHARED = RWL_READ,
    RWL_WRITE,
    RWL_EXCLUSIVE = RWL_WRITE,
    RWL_UNLOCK,
  } mode;

  int lock(mode lock_mode, bool wait = true);
  int unlock();

protected:
  sem_t *m_rd;
  sem_t *m_wr;
  std::string m_name;
  mode m_mode;
};

