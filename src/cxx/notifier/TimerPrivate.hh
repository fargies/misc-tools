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
** TimerPrivate.hh
**
**        Created on: Nov 10, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __TIMER_PRIVATE_HH__
#define __TIMER_PRIVATE_HH__

#include <stdint.h>

#include "DispatcherPrivate.hh"

class Timer;

class TimerPrivate : public RefCounter
{
public:
    /**
     * @brief TimerPrivate constructor.
     *
     * @param[in] timer the Timer it belongs to.
     * @param[in] interval the timer interval.
     */
    TimerPrivate(uint32_t interval, TimerHandler &handler) :
        m_interval(interval), m_handler(&handler)
    {}

    ~TimerPrivate()
    {}

    inline uint32_t getInterval() const
    {
        return m_interval;
    }

    inline void setInterval(uint32_t interval)
    {
        m_interval = interval;
    }

    inline const struct timespec &getTime() const
    {
        return m_time;
    }

    inline void setHandler(TimerHandler &handler)
    {
        m_handler = &handler;
    }

protected:
    struct timespec     m_time;
    uint32_t            m_interval;
    TimerHandler       *m_handler;
    TimerList::iterator m_it;

    friend class DispatcherPrivate;
};

#endif

