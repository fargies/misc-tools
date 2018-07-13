/**
 ** Copyright (C) 2014 fargie_s
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
 ** SignalWaiter.hpp
 **
 **        Created on: 07/12/2014
 **   Original Author: fargie_s
 **
 **/

#ifndef __SIGNAL_WAITER_HPP__
#define __SIGNAL_WAITER_HPP__

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class SignalWaiter : public QObject
{
    Q_OBJECT

public:
    SignalWaiter(QObject *obj, const char *sig);

    /**
     * @brief wait for the event to occur.
     * @param[in] timeout in milliseconds.
     * @return
     *  - true if the event was caught.
     *  - false on timeout.
     */
    bool wait(unsigned long timeout);

    void reset();

protected Q_SLOTS:
    void wake();

protected:
    QObject *m_obj;
    int m_count;
    QMutex m_lock;
    QWaitCondition m_cond;
};

#endif  // __SIGNAL_WAITER_HPP__
