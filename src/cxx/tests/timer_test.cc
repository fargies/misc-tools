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
** timer_test.cc
**
**        Created on: Nov 11, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "notifier/Dispatcher.hh"
#include "notifier/Timer.hh"

using namespace notifier;

class MyTimerHandler : public TimerHandler
{
public:
    int ok;

    bool handle()
    {
        ok = 1;
        return false;
    }

    MyTimerHandler() :
        ok(0)
    {}
};

class TimerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TimerTest);
    CPPUNIT_TEST(test_timer);
    CPPUNIT_TEST(test_timer2);
    CPPUNIT_TEST(test_stop);
    CPPUNIT_TEST(test_restart);
    CPPUNIT_TEST(test_timer_notscheduled);
    CPPUNIT_TEST_SUITE_END();

protected:
    void test_timer()
    {
        Dispatcher receiver;
        MyTimerHandler handler;
        Timer timer(2000, receiver, handler);
        timer.setHandler(handler);

        time_t tt = time(NULL);

        timer.start();
        receiver.dispatch(4000);

        time_t ntt = time(NULL);

        CPPUNIT_ASSERT(handler.ok == 1);
        CPPUNIT_ASSERT((ntt-tt) >= 2);
        CPPUNIT_ASSERT((ntt-tt) < 4);
    }

    void test_timer2()
    {
      Dispatcher receiver;
      MyTimerHandler handler;
      Timer timer(2000, receiver, handler);

      time_t tt = time(NULL);

      timer.start();
      receiver.dispatch(4000);

      time_t ntt = time(NULL);

      CPPUNIT_ASSERT(handler.ok == 1);
      CPPUNIT_ASSERT((ntt-tt) >= 2);
      CPPUNIT_ASSERT((ntt-tt) < 4);
    }

    void test_stop()
    {
        Dispatcher receiver;
        MyTimerHandler handler;
        Timer timer(2000, receiver, handler);
        timer.setHandler(handler);

        timer.start();
        timer.stop();
        receiver.dispatch(4000);

        CPPUNIT_ASSERT(handler.ok == 0);
    }

    void test_restart()
    {
        Dispatcher receiver;
        MyTimerHandler handler;
        Timer timer(2000, receiver, handler);
        timer.setHandler(handler);

        time_t tt = time(NULL);

        timer.start();
        timer.restart();
        receiver.dispatch(4000);

        time_t ntt = time(NULL);
        CPPUNIT_ASSERT(handler.ok == 1);
        CPPUNIT_ASSERT((ntt-tt) > 1);
    }

    void test_timer_notscheduled()
    {
        Dispatcher receiver;
        MyTimerHandler handler;
        Timer timer(2000, receiver, handler);
        timer.setHandler(handler);

        timer.start();
        receiver.dispatch(40);

        CPPUNIT_ASSERT(handler.ok == 0);
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION(TimerTest);

