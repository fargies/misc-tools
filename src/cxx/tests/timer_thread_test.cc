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
** timer_thread_test.cc
**
**        Created on: Nov 11, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "notifier/Dispatcher.hh"
#include "notifier/Timer.hh"
#include "threading/thread.hh"
#include "threading/cond.hh"

/*
 * Simple threaded dispatcher used in the test
 */
class ThreadedDispatcher :
    public Dispatcher,
    public Thread
{
public:
    void start()
    {
        if (state() != Thread::RUNNING)
        {
            m_running = true;
            Thread::start();
        }
    }

    void stop()
    {
        if (state() == Thread::RUNNING)
        {
            m_running = false;
            wake();
            join();
        }
    }

    ThreadedDispatcher() :
        m_running(false)
    {
        start();
    }

    ~ThreadedDispatcher()
    {
        stop();
    }

protected:
    void thread_routine()
    {
        while (m_running == true)
            dispatch(-1);
    }

    bool m_running;
};

class LongTimerHandler : public TimerHandler
{
public:
    LongTimerHandler() :
        _ok(0)
    {}

    bool handle()
    {
        _cond.lock();
        _ok = 1;
        _cond.signal();
        _cond.unlock();

        sleep(2);

        _cond.lock();
        _ok = 2;
        _cond.signal();
        _cond.unlock();
        return false;
    }

    bool wait(int val, int timeout = 5)
    {
        bool ret;

        _cond.lock();
        if (!(ret = (val == _ok)))
        {
            _cond.timedWait(timeout);
            ret = (val == _ok);
        }
        _cond.unlock();
        return ret;
    }

    int get()
    {
        return _ok;
    }

protected:
    Cond _cond;
    int _ok;
};

class RecurseTimerHandler : public TimerHandler
{
public:
    RecurseTimerHandler(Dispatcher &disp) :
        _timer(0, disp, *this), _recur(10)
    {
    }

    bool handle()
    {
        bool ret = false;

        _cond.lock();
        if (--_recur != 0)
        {
            _timer.setHandler(*this);
            _timer.isRunning();
            _timer.setInterval(0);
            ret = true;
        }
        else
            _cond.signal();
        _cond.unlock();

        return ret;
    }

    bool wait()
    {
        bool ret;

        _cond.lock();
        if (!(ret = (_recur == 0)))
        {
            _cond.timedWait(5);
            ret = _recur == 0;
        }
        _cond.unlock();
        return ret;
    }

    void start()
    {
        _timer.start();
    }

protected:
    Timer _timer;
    int _recur;
    Cond _cond;
};

class TimerThreadTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TimerThreadTest);
    CPPUNIT_TEST(test_stop_in_handle);
    CPPUNIT_TEST(test_recurse);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
        m_dispatcher = new ThreadedDispatcher();
    }

    void tearDown()
    {
        delete m_dispatcher;
    }

protected:
    ThreadedDispatcher *m_dispatcher;

    void test_stop_in_handle()
    {
        LongTimerHandler *handler = new LongTimerHandler();
        Timer timer(0, *m_dispatcher, *handler);
        timer.start();

        CPPUNIT_ASSERT(handler->wait(1)); /* dispatch has started */

        timer.stop(true); /* the timer shouldn't be stopped until the dispatch
                             end */

        CPPUNIT_ASSERT_EQUAL(2, handler->get());
        delete handler; /* the timer is stopped, it shouldn't segfault */
    }

    void test_recurse()
    {
        RecurseTimerHandler handler(*m_dispatcher);
        handler.start();

        CPPUNIT_ASSERT(handler.wait());
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimerThreadTest);


