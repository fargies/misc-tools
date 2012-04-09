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
** thread.cc
**
**        Created on: Apr 08, 2012
**   Original Author: fargie_s
**
*/

#include <errno.h>
#include <signal.h>
#include "thread.hh"

#ifdef THREAD_SAFE

#include "locker.hh"

#define TCOND_LOCKER(cond) Locker l ## __LINE__(cond)
#define TCOND_BROADCAST(cond) cond.broadcast()
#define TCOND_WAIT(cond) cond.wait()
#else
#define TCOND_LOCKER(cond) do {} while (0)
#define TCOND_BROADCAST(cond) do {} while (0)
#define TCOND_WAIT(cond) do {} while (0)
#endif

Thread::Thread() :
    m_thread_state(STOPPED)
{
}

Thread::~Thread()
{
}

void *Thread::thread_routine_wrapper(void *data)
{
    Thread *thread = static_cast<Thread *>(data);

    thread->thread_routine();

    TCOND_LOCKER(thread->m_thread_cond);
    if (thread->m_thread_state == DETACHED)
        thread->m_thread_state = STOPPED;
    TCOND_BROADCAST(thread->m_thread_cond);

    pthread_exit(0);
}

int Thread::start()
{
    TCOND_LOCKER(m_thread_cond);

    if (m_thread_state != STOPPED)
        return EEXIST;

    int rc = pthread_create(&m_thread_id, NULL, thread_routine_wrapper,
            static_cast<void *>(this));
    if (rc == 0)
        m_thread_state = RUNNING;
    return rc;
}

int Thread::join()
{
    TCOND_LOCKER(m_thread_cond);

    switch (m_thread_state)
    {
        case RUNNING:
            {
                TCOND_WAIT(m_thread_cond);

                int rc = 0;

                if (m_thread_state == RUNNING)
                {
                    void *value_ptr;
                    rc = pthread_join(m_thread_id, &value_ptr);
                }
                m_thread_state = STOPPED;

                return rc;
            }
        case DETACHED:
            return EINVAL;
        case STOPPED:
            return ESRCH;
    };
}

int Thread::detach()
{
    TCOND_LOCKER(m_thread_cond);

    switch (m_thread_state)
    {
        case RUNNING:
            {
                int rc = pthread_detach(m_thread_id);
                if (rc == 0)
                    m_thread_state = DETACHED;

                return rc;
            }
        case DETACHED:
            return EINVAL;
        case STOPPED:
            return ESRCH;
    };
}

int Thread::kill(int sig)
{
    TCOND_LOCKER(m_thread_cond);

    if (m_thread_state == STOPPED)
        return ESRCH;

    return pthread_kill(m_thread_id, sig);
}

Thread::State Thread::state() const
{
    TCOND_LOCKER(m_thread_cond);
    return m_thread_state;
}

