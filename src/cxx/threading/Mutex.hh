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
** Mutex.hh
**
**        Created on: Nov 13, 2011
**   Original Author: fargie_s
**
*/

#ifndef __MUTEX_HH__
#define __MUTEX_HH__

#include <pthread.h>

#ifdef NDEBUG
#define DEFAULT_MUTEX_TYPE PTHREAD_MUTEX_DEFAULT
#else
#define DEFAULT_MUTEX_TYPE PTHREAD_MUTEX_ERRORCHECK
#endif

namespace threading {

class Mutex
{
public:
    Mutex(int type = DEFAULT_MUTEX_TYPE);
    ~Mutex();

    void lock();
    bool trylock();
    void unlock();

protected:
    pthread_mutex_t m_mutex;

private:
    Mutex(const Mutex &);
    Mutex &operator =(const Mutex &);
};

}

#endif

