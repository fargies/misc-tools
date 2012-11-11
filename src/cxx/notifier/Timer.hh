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
** Timer.hh
**
**        Created on: Nov 10, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __TIMER_HH__
#define __TIMER_HH__

#include <stdint.h>

#include "TimerHandler.hh"
#include "autoRef.hh"

class Dispatcher;
class TimerPrivate;

class Timer
{
public:
    /**
     * @brief timer constructor.
     *
     * @param[in] timeout timer duration in milliseconds.
     * @param[in] dispatcher timer's dispatcher.
     * @param[in] handler timer handler.
     */
    Timer(
            uint32_t timeout,
            Dispatcher &dispatcher,
            TimerHandler &handler);
    ~Timer();

    /**
     * @brief set the timer handler.
     *
     * @warning the timer must be stopped or in it's handler while calling this
     * method.
     */
    void setHandler(
            TimerHandler &handler);

    /**
     * @brief start the timer.
     *
     * @note the timer is not automatically started when created.
     */
    void start();

    /**
     * @brief stop the timer.
     *
     * @param[in] wait wait until the end of the Timer's callback (if it's
     * running).
     */
    void stop(bool wait = true);

    /**
     * @brief restart the timer from the begining.
     */
    void restart();

    /**
     * @brief modify the timeout value.
     */
    void setInterval(uint32_t timeout);

    /**
     * @return true if the timer has been started and hasn't returned yet.
     * false otherwise.
     */
    bool isRunning();

protected:
    AutoRef<TimerPrivate> m_ptimer;
    Dispatcher &m_disp;
};

#endif

