/*
** This document and/or file is SOMFY's property. All information it
** contains is strictly confidential. This document and/or file shall
** not be used, reproduced or passed on in any way, in full or in part
** without SOMFY's prior written approval. All rights reserved.
** Ce document et/ou fichier est la propritye SOMFY. Les informations
** quil contient sont strictement confidentielles. Toute reproduction,
** utilisation, transmission de ce document et/ou fichier, partielle ou
** intégrale, non autorisée préalablement par SOMFY par écrit est
** interdite. Tous droits réservés.
** 
** Copyright © (2009-2012), Somfy SAS. All rights reserved.
** All reproduction, use or distribution of this software, in whole or
** in part, by any means, without Somfy SAS prior written approval, is
** strictly forbidden.
**
** RefCnt.hh
**
**        Created on: Aug 27, 2012
**   Original Author: Sylvain Fargier <sylvain.fargier@somfy.com>
**
*/

#include <map>
#include <list>
#include <deque>

#include <sys/eventfd.h> // Linux specific
#include <sys/timerfd.h> // Linux specific

#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include <poll.h>

#include "Dispatcher.hh"
#include "FDWatch.hh"
#include "EventHandler.hh"
#include "TimerPrivate.hh"
#include "TimerHandler.hh"

#include "threading/locker.hh"
#include "threading/mutex.hh"
#include "refCounter.hh"
#include "autoPtr.hh"
#include "timespec.hh"

using namespace std;

/**
 * @brief comparison operator to order the TimerList
 */
static bool timer_cmp(
        const AutoRef<TimerPrivate> &p1,
        const AutoRef<TimerPrivate> &p2)
{
    if (p1->getTime() == p2->getTime())
        return p1.get() < p2.get();
    else
        return p1->getTime() < p2->getTime();
}

DispatcherPrivate::DispatcherPrivate() :
    m_timers(timer_cmp),
    m_lock(PTHREAD_MUTEX_RECURSIVE)
{
    m_wakefd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    m_timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK |
            TFD_CLOEXEC);
}

DispatcherPrivate::~DispatcherPrivate()
{
    close(m_wakefd);
    close(m_timerfd);
}

void DispatcherPrivate::wake()
{
    uint64_t i = 1;
    write(m_wakefd, &i, sizeof (i));
}

void DispatcherPrivate::startTimer(TimerPrivate &ev)
{
    Locker l(m_timers_lock);
    if (ev.m_it != m_timers.end())
        m_timers.erase(ev.m_it);

    addMilisecs(getCurrentTime(ev.m_time), ev.getInterval());
    ev.m_it = m_timers.insert(m_timers.end(), AutoRef<TimerPrivate>(&ev));

    if (m_timers.begin() == ev.m_it)
        setNextTimer(ev.getTime());
}

void DispatcherPrivate::stopTimer(TimerPrivate &ev, bool wait)
{
    Locker l(m_timers_lock);
    if (wait && isInHandler(ev))
        m_timers_lock.wait();
    if (ev.m_it != m_timers.end())
    {
        m_timers.erase(ev.m_it);
        ev.m_it = m_timers.end();
    }
}

void DispatcherPrivate::initTimer(TimerPrivate &timer)
{
    timer.m_it = m_timers.end();
}

bool DispatcherPrivate::isRunning(TimerPrivate &timer)
{
    Locker l(m_timers_lock);
    return (timer.m_it != m_timers.end()) || isInHandler(timer);
}

bool DispatcherPrivate::isInHandler(TimerPrivate &timer)
{
    return (&timer == m_curr_timer);
}

void DispatcherPrivate::setNextTimer(const struct timespec &ts)
{
    struct itimerspec its;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    its.it_value = ts;

    timerfd_settime(m_timerfd, TFD_TIMER_ABSTIME, &its, NULL);
}

Dispatcher::Dispatcher() :
    m_pdisp(new DispatcherPrivate())
{
}

Dispatcher::~Dispatcher()
{
}

Dispatcher::Dispatcher(const Dispatcher &disp) :
    m_pdisp(disp.m_pdisp)
{
}

Dispatcher &Dispatcher::operator =(const Dispatcher &disp)
{
    if (this != &disp)
    {
        m_pdisp = disp.m_pdisp;
    }
    return *this;
}

void Dispatcher::post(EventBase *evt)
{
    Locker l(m_pdisp->m_lock);
    m_pdisp->m_events.push_front(AutoRef<EventBase>(evt));
    m_pdisp->wake();
}

void Dispatcher::addHandler(EventHandler *handler, EventBase::type type)
{
    if (handler == NULL)
        return;

    Locker l(m_pdisp->m_lock);
    m_pdisp->m_evt_hdl.insert(make_pair(type, handler));
}

void Dispatcher::removeHandler(EventHandler *handler, EventBase::type type)
{
    Locker l(m_pdisp->m_lock);

    EventHandlerMap &hdlrs = m_pdisp->m_evt_hdl;
    EventHandlerMap::iterator it = hdlrs.find(type);

    for (; it != hdlrs.end(); ++it)
        if (it->first != type)
            break;
        else if (it->second == handler)
        {
            hdlrs.erase(it);
            break;
        }
}

void Dispatcher::addHandler(FDWatch *handler, int fd, FDWatch::direction dir)
{
    if (handler == NULL)
        return;

    Locker l(m_pdisp->m_lock);
    m_pdisp->m_fd_hdl[fd] = new FDWatchDesc(handler, dir);
    m_pdisp->wake();
}

void Dispatcher::removeHandler(FDWatch *handler, int fd, FDWatch::direction dir)
{
    Locker l(m_pdisp->m_lock);

    FDWatchMap &hdlrs = m_pdisp->m_fd_hdl;
    FDWatchMap::iterator it = hdlrs.find(fd);

    if ((it->second->hdl == handler) && (it->second->dir == dir))
        hdlrs.erase(it);
}

int Dispatcher::dispatch(int timeout)
{
    m_pdisp->dispatch(timeout);
}

void Dispatcher::wake()
{
    m_pdisp->wake();
}

struct timespec &DispatcherPrivate::getCurrentTime(struct timespec &t)
{
    if (clock_gettime(CLOCK_MONOTONIC, &t) < 0)
    {
        //FIXME warn here
        time_t ti = ::time(NULL);
        t.tv_sec = ti;
        t.tv_nsec = 0;
    }
    return t;
}

int DispatcherPrivate::handleEvent()
{
    int nb = 0;

    Locker l(m_lock);
    while (!m_events.empty())
    {
        AutoRef<EventBase> evt(m_events.back());
        m_events.pop_back();


        EventHandlerMap::const_iterator it = m_evt_hdl.find(evt->getType());
        for (; (it != m_evt_hdl.end()); ++it)
            it->second->handle(*evt);
        ++nb;
    }
    return nb;
}

int DispatcherPrivate::handleTimer()
{
    int nb = 0;
    struct timespec ts;
    getCurrentTime(ts);

    m_timers_lock.lock();
    while (!m_timers.empty())
    {
        TimerList::iterator it = m_timers.begin();
        if ((*it)->getTime() <= ts)
        {
            m_curr_timer = *it;
            m_timers.erase(it);
            m_curr_timer->m_it = m_timers.end();
            TimerHandler *handler = m_curr_timer->m_handler;
            /* the timer has been removed and im_curr_timer has a ref
             * on it */
            m_timers_lock.unlock();
            bool restart = handler && handler->handle();

            m_timers_lock.lock();
            if (restart)
            {
                addMilisecs(getCurrentTime(m_curr_timer->m_time),
                        m_curr_timer->getInterval());
                m_curr_timer->m_it = m_timers.insert(m_timers.end(),
                        m_curr_timer);
            }
            m_timers_lock.broadcast();
            m_curr_timer = NULL;
            ++nb;
        }
        else
        {
            setNextTimer((*it)->getTime());
            break;
        }
    }
    m_timers_lock.unlock();
    return nb;
}

int DispatcherPrivate::dispatch(int timeout)
{
    m_lock.lock();
    size_t nfds = 2 + m_fd_hdl.size();
    int rc;

    struct pollfd fds[nfds];

    {
        FDWatchMap::const_iterator it = m_fd_hdl.begin();
        for (nfds = 2; it != m_fd_hdl.end(); ++it)
        {
            fds[nfds].fd = it->first;
            fds[nfds].events = POLLERR | POLLHUP;
            if (it->second->dir & FDWatch::INPUT)
                fds[nfds].events |= POLLIN;
            if (it->second->dir & FDWatch::OUTPUT)
                fds[nfds].events |= POLLOUT;
            fds[nfds++].revents = 0;
        }
    }
    m_lock.unlock();

    fds[0].fd = m_wakefd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd = m_timerfd;
    fds[1].events = POLLIN;
    fds[1].revents = 0;

    rc = poll(fds, nfds, timeout);
    if (rc > 0)
    {
        int nb = 0;

        if (fds[0].revents != 0) /* we got a manual wake-up */
        {
            uint64_t i;
            read(m_wakefd, &i, sizeof (i));

            nb += handleEvent();
            --rc;
        }
        if (fds[1].revents != 0) /* there's a timer */
        {
            uint64_t i;
            read(m_timerfd, &i, sizeof (i));

            nb += handleTimer();
            --rc;
        }
        while ((nfds-- > 2) && (rc != 0))
        {
            if (fds[nfds].revents != 0)
            {
                Locker l(m_lock);
                FDWatchMap::const_iterator it = m_fd_hdl.find(fds[nfds].fd);
                for (; (it != m_fd_hdl.end()) && (it->first == fds[nfds].fd); ++it)
                    it->second->hdl->handle(fds[nfds].fd);

                --rc;
                ++nb;
            }
        }
        return nb;
    }
    else if (rc == 0)
        return -1;
    else if (rc == EINTR)
        return -2;
    else
        return -3;
}

