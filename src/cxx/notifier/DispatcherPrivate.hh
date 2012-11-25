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
** DispatcherPrivate.hh
**
**        Created on: Nov 10, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __DISPATCHER_PRIVATE_HH__
#define __DISPATCHER_PRIVATE_HH__

#include <time.h>
#include <stdint.h>
#include <map>
#include <set>
#include <deque>

#include "AutoPtr.hh"
#include "RefCounter.hh"
#include "threading/Cond.hh"

#include "Dispatcher.hh"
#include "Event.hh"
#include "Timer.hh"

namespace notifier {

struct FDWatchDesc
{
    FDWatchDesc(FDWatch *h, FDWatch::direction d) :
        hdl(h), dir(d)
    {}

    FDWatch *hdl;
    FDWatch::direction dir;
};

typedef std::multimap<EventBase::type, EventHandler *> EventHandlerMap;
typedef std::map<int, AutoPtr<FDWatchDesc> > FDWatchMap;
typedef std::set<AutoRef<TimerPrivate>, bool (*)(
        const AutoRef<TimerPrivate> &,
        const AutoRef<TimerPrivate> &) > TimerList;
typedef std::deque<AutoRef<EventBase> > EventQueue;

class DispatcherPrivate : public RefCounter
{
public:
    DispatcherPrivate();
    ~DispatcherPrivate();

    void wake();
    int dispatch(int timeout);

    void initTimer(TimerPrivate &ev);
    void startTimer(TimerPrivate &ev);
    void stopTimer(TimerPrivate &ev, bool wait = true);
    void setTimerInterval(TimerPrivate &ev, uint32_t interval);

    /**
     * @brief check if the given timer is running.
     *
     * @return true if the timer is running (in the timer queue or in it's
     * handler).
     */
    bool isRunning(TimerPrivate &timer);

    /**
     * @brief get reference clock time.
     * @details a monotonic clock is used as reference.
     *
     * @param[out] ts the current time.
     */
    struct timespec &getCurrentTime(struct timespec &ts);

    inline threading::Cond &getTimerLock()
    {
        return m_timers_lock;
    }

    bool inDispatcher();

protected:
    void setNextTimer(const struct timespec &ts);
    int handleTimer();
    int handleEvent();

    /**
     * @brief check if the given timer is in it's timer handler.
     *
     * @warning this method must be called with m_timers_lock mutex held.
     */
    bool isInHandler(TimerPrivate &timer);

    EventHandlerMap m_evt_hdl;
    FDWatchMap m_fd_hdl;
    EventQueue m_events;
    TimerList m_timers;
    AutoRef<TimerPrivate> m_curr_timer;

    threading::Mutex m_lock;
    threading::Cond m_timers_lock;

    int m_wakefd;
    int m_timerfd;
    bool m_dispatch;
    pthread_t m_disp_th;

    friend class Dispatcher;
};

}

#endif

