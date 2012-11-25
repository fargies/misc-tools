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
** key_test.cc
**
**        Created on: Nov 25, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "threading/Thread.hh"
#include "threading/Key.hh"

using namespace threading;

class ThreadObj
{
public:
    ThreadObj(bool &del) :
        m_del(del)
    {
        m_del = false;
    }

    ~ThreadObj()
    {
        m_del = true;
    }

protected:
    bool &m_del;
};

class KeyThread : public Thread
{
public:
    KeyThread(Key<ThreadObj> &k) :
        m_key(k)
    {
        obj_deleted = false;
    }

    void thread_routine()
    {
        m_key = new ThreadObj(obj_deleted);
    }

    Key<ThreadObj> &m_key;
    bool obj_deleted;
};

class TestKey : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestKey);
    CPPUNIT_TEST(simple);
    CPPUNIT_TEST(override);
    CPPUNIT_TEST(access);
    CPPUNIT_TEST(thread_obj);
    CPPUNIT_TEST_SUITE_END();

public:
    void simple()
    {
        bool deleted = false;

        {
            Key<ThreadObj> k;

            k = new ThreadObj(deleted);
            CPPUNIT_ASSERT(deleted == false);
        }
        CPPUNIT_ASSERT(deleted == true);
    }

    void override()
    {
        bool del = false, del2 = false;

        {
            Key<ThreadObj> k;

            k = new ThreadObj(del);
            CPPUNIT_ASSERT(del == false);

            k = new ThreadObj(del2);
            CPPUNIT_ASSERT(del == true);
        }
        CPPUNIT_ASSERT(del2 == true);
    }

    void access()
    {
        Key<int> k;

        CPPUNIT_ASSERT(k == NULL);
        CPPUNIT_ASSERT(NULL == k);
        k = NULL;

        int *i = new int(42);
        k = i;

        CPPUNIT_ASSERT(k == i);
        CPPUNIT_ASSERT(i == k);
        CPPUNIT_ASSERT(k != NULL);
        CPPUNIT_ASSERT(NULL != k);

        CPPUNIT_ASSERT(*k == 42);
    }

    void thread_obj()
    {
        Key<ThreadObj> key;
        KeyThread thread(key);

        thread.start();
        thread.join();
        CPPUNIT_ASSERT(thread.obj_deleted);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestKey);
