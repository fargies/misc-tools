/*
** Copyright (C) 2012 Sylvain Fargier <sylvain.fargier@somfy.com>
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
** autoptr_test.cc
**
**        Created on: May 09, 2011
**   Original Author: Sylvain Fargier <sylvain.fargier@somfy.com>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "autoptr.hh"

class TestAutoPtrObject
{
public:
    TestAutoPtrObject(bool &del) :
        m_del(del)
    {
        m_del = false;
    }

    ~TestAutoPtrObject()
    {
        m_del = true;
    }

    /* this method is a nonsense but we need a method */
    inline bool isDel() const {
        return m_del;
    }
protected:
    bool &m_del;
};

class TestAutoPtr : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestAutoPtr);
    CPPUNIT_TEST(simple);
    CPPUNIT_TEST(scope);
    CPPUNIT_TEST(overwrite);
    CPPUNIT_TEST_SUITE_END();

public:
    void simple()
    {
        bool del = false;

        AutoPtr<TestAutoPtrObject> ptr(new TestAutoPtrObject(del));

        CPPUNIT_ASSERT(!ptr->isDel());
        CPPUNIT_ASSERT(!(*ptr).isDel());
        CPPUNIT_ASSERT(ptr.get() != NULL);

        ptr = NULL;
        CPPUNIT_ASSERT(del);
        CPPUNIT_ASSERT(ptr == NULL);
        CPPUNIT_ASSERT(NULL == ptr);
        CPPUNIT_ASSERT(ptr.get() == NULL);
    }

    void scope()
    {
        bool del = false;

        {
            AutoPtr<TestAutoPtrObject> ptr(new TestAutoPtrObject(del));
        }
        CPPUNIT_ASSERT(del);

        del = false;
        {
            AutoPtr<TestAutoPtrObject> ptr(new TestAutoPtrObject(del));
            {
                AutoPtr<TestAutoPtrObject> ptr2(ptr);
                /* steal the pointer */
            }
            CPPUNIT_ASSERT(del);
            CPPUNIT_ASSERT(ptr == NULL);
        }
    }

    void overwrite()
    {
        bool del = false, del2 = false;

        AutoPtr<TestAutoPtrObject> ptr(new TestAutoPtrObject(del));
        ptr = new TestAutoPtrObject(del2);
        CPPUNIT_ASSERT(del);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestAutoPtr);

