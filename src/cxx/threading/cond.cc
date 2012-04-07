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
** cond.cc
**
**        Created on: Apr 06, 2012
**   Original Author: fargie_s
**
*/

#include <time.h>
#include "cond.hh"

#ifdef NDEBUG
#define CHECK_EQ0(cmd) cmd
#else

#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#define CHECK_EQ0(cmd) { \
    int rc = (cmd); \
    if (rc != 0) { \
        fprintf(stderr, "%s:%i cmd:%s failed rc:%i:%s", \
                __FILE__, __LINE__, #cmd, rc, strerror(rc)); \
        assert(0); \
    } \
}
#endif

Cond::Cond() :
    Mutex()
{
    CHECK_EQ0(pthread_cond_init(&m_cond, NULL));
}

Cond::~Cond()
{
    CHECK_EQ0(pthread_cond_destroy(&m_cond));
}

void Cond::wait()
{
    CHECK_EQ0(pthread_cond_wait(&m_cond, &m_mutex));
}

int Cond::timedWait(int timeout)
{
    timespec t;

    CHECK_EQ0(clock_gettime(CLOCK_REALTIME, &t));

    unsigned long nsec = (timeout % 1000) * (1000 * 1000) + t.tv_nsec;
    t.tv_sec += timeout / 1000;

    if (nsec >= 1000000000)
    {
        t.tv_sec += 1;
        nsec -= 1000000000;
    }
    t.tv_nsec = nsec;

    return pthread_cond_timedwait(&m_cond, &m_mutex, &t);
}

void Cond::signal()
{
    CHECK_EQ0(pthread_cond_signal(&m_cond));
}

void Cond::broadcast()
{
    CHECK_EQ0(pthread_cond_broadcast(&m_cond));
}

