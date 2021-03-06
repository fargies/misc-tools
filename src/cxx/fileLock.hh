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
** fileLock.hh
**
**        Created on: Sat Feb  5 15:10:20 2011
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __FILELOCK_HH__
#define __FILELOCK_HH__

class FileLock {
public:

  typedef enum {
    FL_RD,
    FL_SH = FL_RD,
    FL_WR,
    FL_EX = FL_WR,
    FL_UNLOCK,
  } mode;

  FileLock(int fd);
  ~FileLock();

  int lock(mode m, bool wait = true);
  int unlock();

protected:
  int m_fd;
  mode m_mode;

  FileLock(const FileLock &);
};

#endif

