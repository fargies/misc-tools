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
** sem.hh
**
**        Created on: Feb 5, 2011
**            Author: fargie_s
**
*/

#ifndef SEM_HH_
#define SEM_HH_

#include <semaphore.h>
#include <errno.h>
#include <string>

class Sem
{
public:
	Sem(int pshared = 0, unsigned int value = 1);
	~Sem();

	int wait();
	int trywait();
	int timedwait(unsigned int msecs);
	int post();
	int getValue();

protected:
	sem_t *m_sem;
};

class NamedSem : public Sem
{
public:
  NamedSem(const std::string &name, unsigned int value = 1);
  ~NamedSem();

  const std::string &getName();
  void unlink();
  static int unlink(const std::string &name);

protected:
  std::string m_name;
  bool m_unlink;
};

#endif /* SEM_HH_ */
