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
** Cond.hh
**
**        Created on: Apr 06, 2012
**   Original Author: fargie_s
**
*/

#ifndef __COND_HH__
#define __COND_HH__

#include <pthread.h>
#include "Mutex.hh"

namespace threading {

class Cond : public Mutex
{
public:
    Cond();
    ~Cond();

    void wait();
    int timedWait(int timeout);
    void signal();
    void broadcast();

protected:
    pthread_cond_t m_cond;

private:
    Cond(const Cond &);
    Cond &operator =(const Cond &);
};

}

#endif

