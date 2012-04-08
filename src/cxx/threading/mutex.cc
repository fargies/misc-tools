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
** mutex.cc
**
**        Created on: Nov 13, 2011
**   Original Author: fargie_s
**
*/

#include "mutex.hh"

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
        fprintf(stderr, "%s:%i cmd:%s failed rc:%i:%s\n", \
                __FILE__, __LINE__, #cmd, rc, strerror(rc)); \
        assert(0); \
    } \
}
#endif

Mutex::Mutex(int type)
{
    pthread_mutexattr_t attrs;
    CHECK_EQ0(pthread_mutexattr_init(&attrs));
    CHECK_EQ0(pthread_mutexattr_settype(&attrs, type));

    CHECK_EQ0(pthread_mutex_init(&m_mutex, &attrs));

    CHECK_EQ0(pthread_mutexattr_destroy(&attrs));
}

Mutex::~Mutex()
{
    CHECK_EQ0(pthread_mutex_destroy(&m_mutex));
}

bool Mutex::trylock()
{
    int rc = pthread_mutex_trylock(&m_mutex);

    if (rc == 0)
        return true;
    else if (rc == EBUSY)
        return false;
    else
    {
        CHECK_EQ0(rc);
        return false;
    }
}

void Mutex::lock()
{
    CHECK_EQ0(pthread_mutex_lock(&m_mutex));
}

void Mutex::unlock()
{
    CHECK_EQ0(pthread_mutex_unlock(&m_mutex));
}

