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
** thread.hh
**
**        Created on: Apr 08, 2012
**   Original Author: fargie_s
**
*/

#ifndef __THREAD_HH__
#define __THREAD_HH__

#include <pthread.h>

#define THREAD_SAFE

#ifdef THREAD_SAFE
#include "cond.hh"
#endif

class Thread
{
public:
    typedef enum {
        STOPPED,
        RUNNING,
        DETACHED
    } State;

    Thread();
    virtual ~Thread();

    int start();

    int join();

    int detach();

    State state() const;

    /**
     * @brief send a signal to the running thread.
     * @details calls pthread_kill.
     */
    int kill(int);

protected:
    virtual void thread_routine() = 0;

private:
    pthread_t m_thread_id;
#ifdef THREAD_SAFE
    mutable Cond m_thread_cond;
#endif
    State m_thread_state;

    static void *thread_routine_wrapper(void *data);
};

#endif

