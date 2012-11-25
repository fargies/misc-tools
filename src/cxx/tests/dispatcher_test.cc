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
** dispatcher_test.cc
**
**        Created on: Nov 10, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "notifier/Dispatcher.hh"
#include "notifier/Event.hh"

using namespace notifier;

class TestEvent : public Event<TestEvent>
{
public:
    TestEvent(bool &deleted) :
        m_deleted(deleted)
    {
        m_deleted = false;
    }

    ~TestEvent()
    {
        m_deleted = true;
    }

protected:
    bool &m_deleted;
};

class TestEventHandler : public EventHandler
{
public:
    TestEventHandler(Dispatcher &disp) :
        m_disp(disp), m_count(0)
    {
        m_disp.addHandler(this, TestEvent::s_type);
    }

    ~TestEventHandler()
    {
        m_disp.removeHandler(this, TestEvent::s_type);
    }

    void handle(EventBase &evt)
    {
        ++m_count;
    }

    int getCount()
    {
        return m_count;
    }

protected:
    Dispatcher &m_disp;
    int m_count;
};

class TestDispatcher : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestDispatcher);
    CPPUNIT_TEST(simple);
    CPPUNIT_TEST(timeout);
    CPPUNIT_TEST(double_hdlr);
    CPPUNIT_TEST_SUITE_END();

public:
    void simple()
    {
        bool deleted = true;
        Dispatcher disp;
        TestEventHandler hdl(disp);

        disp.post(new TestEvent(deleted));
        CPPUNIT_ASSERT(hdl.getCount() == 0);
        CPPUNIT_ASSERT(deleted == false);

        CPPUNIT_ASSERT(disp.dispatch(0) == 1);
        CPPUNIT_ASSERT(hdl.getCount() == 1);
        CPPUNIT_ASSERT(deleted == true);

        deleted = false;
        TestEvent *evt = new TestEvent(deleted);
        evt->incRef();
        for (int i = 0; i < 10; ++i)
            disp.post(evt);
        evt->decRef();
        CPPUNIT_ASSERT(evt->getNumRef() == 10);

        CPPUNIT_ASSERT(disp.dispatch(0) == 10);
        CPPUNIT_ASSERT(hdl.getCount() == 11);
        CPPUNIT_ASSERT(deleted == true);
    }

    void timeout()
    {
        Dispatcher disp;

        CPPUNIT_ASSERT(disp.dispatch(10) == -1);
    }

    void double_hdlr()
    {
        bool deleted = false;
        Dispatcher disp;
        TestEventHandler hdl1(disp);
        TestEventHandler hdl2(disp);

        disp.post(new TestEvent(deleted));
        CPPUNIT_ASSERT(disp.dispatch(0) == 1);
        CPPUNIT_ASSERT(hdl1.getCount() == 1);
        CPPUNIT_ASSERT(hdl2.getCount() == 1);

        CPPUNIT_ASSERT(disp.dispatch(1) == -1);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestDispatcher);

