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
** Dispatcher.hh
**
**        Created on: Nov 09, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __DISPATCHER_HH__
#define __DISPATCHER_HH__

#include "AutoRef.hh"
#include "EventHandler.hh"
#include "Event.hh"
#include "FDWatch.hh"

namespace notifier {

class DispatcherPrivate;

class Dispatcher
{
public:
    Dispatcher();
    ~Dispatcher();

    Dispatcher(const Dispatcher &disp);
    Dispatcher &operator =(const Dispatcher &disp);

    void post(EventBase *evt);

    void addHandler(EventHandler *handler, EventBase::type type);
    void removeHandler(EventHandler *handler, EventBase::type type);

    void addHandler(FDWatch *handler, int fd, FDWatch::direction dir);
    void removeHandler(FDWatch *handler, int fd, FDWatch::direction dir);

    int dispatch(int timeout);

    /**
     * @brief check if current thread is in the dispatcher context.
     *
     * @return
     *  - true if the caller is in the dispatcher.
     *  - false otherwise.
     */
    bool inDispatcher();

    void wake();

protected:
    AutoRef<DispatcherPrivate> m_pdisp;

    friend class Timer;
};

}

#endif

