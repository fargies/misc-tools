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
** Timer.cc
**
**        Created on: Nov 11, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include "Timer.hh"
#include "TimerPrivate.hh"

#include "Dispatcher.hh"
#include "DispatcherPrivate.hh"

#include "threading/locker.hh"

TimerHandler::~TimerHandler()
{}

Timer::Timer(
        uint32_t timeout,
        Dispatcher &dispatcher,
        TimerHandler &handler) :
    m_ptimer(new TimerPrivate(timeout, handler)),
    m_disp(dispatcher)
{
    m_disp.m_pdisp->initTimer(*m_ptimer);
}

Timer::~Timer()
{
    m_disp.m_pdisp->stopTimer(*m_ptimer);
}

void Timer::start()
{
    m_disp.m_pdisp->startTimer(*m_ptimer);
}

void Timer::stop(bool wait)
{
    m_disp.m_pdisp->stopTimer(*m_ptimer, wait);
}

void Timer::restart()
{
    m_disp.m_pdisp->startTimer(*m_ptimer);
}

void Timer::setInterval(uint32_t timeout)
{
    Locker l(m_disp.m_pdisp->getTimerLock());
    m_ptimer->setInterval(timeout);
}

void Timer::setHandler(TimerHandler &handler)
{
    Locker l(m_disp.m_pdisp->getTimerLock());
    m_ptimer->setHandler(handler);
}

bool Timer::isRunning()
{
    return m_disp.m_pdisp->isRunning(*m_ptimer);
}

