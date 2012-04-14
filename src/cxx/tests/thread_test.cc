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
** thread_test.cc
**
**        Created on: Apr 09, 2012
**   Original Author: fargie_s
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <errno.h>

#include "threading/thread.hh"

class SimpleThread : public Thread
{
public:
    SimpleThread() :
        m_called(false)
    {}

    bool called()
    {
        return m_called;
    }

protected:
    void thread_routine()
    {
        m_called = true;
    }

    bool m_called;
};

class TestThread : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestThread);
    CPPUNIT_TEST(simple);
    CPPUNIT_TEST_SUITE_END();

public:
    void simple()
    {
        SimpleThread s;

        CPPUNIT_ASSERT_EQUAL(0, s.start());

        CPPUNIT_ASSERT_EQUAL(0, s.join());
        CPPUNIT_ASSERT(s.called());

        CPPUNIT_ASSERT_EQUAL(ESRCH, s.join());
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestThread);