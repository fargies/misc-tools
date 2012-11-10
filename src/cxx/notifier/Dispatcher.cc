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
#include <sys/eventfd.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include <poll.h>

#include "Dispatcher.hh"
#include "refCounter.hh"
#include "threading/locker.hh"
#include "threading/mutex.hh"
#include "FDWatch.hh"
#include "EventHandler.hh"
#include "autoPtr.hh"

using namespace std;

struct FDWatchDesc
{
    FDWatchDesc(FDWatch *h, FDWatch::direction d) :
        hdl(h), dir(d)
    {}

    FDWatch *hdl;
    FDWatch::direction dir;
};

typedef multimap<EventBase::type, EventHandler *> EventHandlerMap;
typedef map<int, AutoPtr<FDWatchDesc> > FDWatchMap;

class DispatcherPrivate : public RefCounter
{
public:
    DispatcherPrivate() :
        m_lock(PTHREAD_MUTEX_RECURSIVE)
    {
        m_wakefd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    }

    ~DispatcherPrivate()
    {
        close(m_wakefd);
    }

    void wake()
    {
        uint64_t i = 1;
        write(m_wakefd, &i, sizeof (i));
    }

    int dispatch(int timeout);

    multimap<EventBase::type, EventHandler *> m_evt_hdl;
    map<int, AutoPtr<FDWatchDesc> > m_fd_hdl;
    deque<AutoRef<EventBase> > m_events;
    Mutex m_lock;
    int m_wakefd;
};


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

int DispatcherPrivate::dispatch(int timeout)
{
    m_lock.lock();
    size_t nfds = 1 + m_fd_hdl.size();
    int rc;

    struct pollfd fds[nfds];

    {
        FDWatchMap::const_iterator it = m_fd_hdl.begin();
        for (nfds = 1; it != m_fd_hdl.end(); ++it)
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

    rc = poll(fds, nfds, timeout);
    if (rc > 0)
    {
        int nb = 0;

        if (fds[0].revents) /* we got a manual wake-up */
        {
            uint64_t i;
            read(m_wakefd, &i, sizeof (i));

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
            --rc;
        }
        while ((nfds-- > 1) && (rc != 0))
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

