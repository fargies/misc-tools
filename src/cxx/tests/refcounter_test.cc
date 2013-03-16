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
** refcounter_test.cc
**
**        Created on: Nov 07, 2012
**   Original Author: Sylvain Fargier <sylvain.fargier@somfy.com>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "RefCounter.hh"
#include "AutoRef.hh"

class RefCountedObject : public RefCounter
{
public:
    RefCountedObject(bool &del) :
        m_del(del)
    {
        m_del = false;
    }

    ~RefCountedObject()
    {
        m_del = true;
    }

protected:
    bool &m_del;
};


class TestRefCounter : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestRefCounter);
    CPPUNIT_TEST(simple);
    CPPUNIT_TEST(onstack);
    CPPUNIT_TEST(autoref);
    CPPUNIT_TEST_SUITE_END();

public:
    void simple()
    {
        bool deleted;
        RefCountedObject *obj = new RefCountedObject(deleted);
        CPPUNIT_ASSERT(obj->getNumRef() == 0);
        CPPUNIT_ASSERT(!deleted);

        obj->incRef();
        CPPUNIT_ASSERT(obj->getNumRef() == 1);

        obj->decRef();
        CPPUNIT_ASSERT(deleted);
    }

    void onstack()
    {
        bool deleted;
        {
            RefCountedObject obj(deleted);
            CPPUNIT_ASSERT(!deleted);
            obj.incRef();
            obj.decRef(false);
        }
        CPPUNIT_ASSERT(deleted);
    }

    void autoref()
    {
        bool deleted;
        {
            AutoRef<RefCountedObject> obj(new RefCountedObject(deleted));
            CPPUNIT_ASSERT(!deleted);
        }
        CPPUNIT_ASSERT(deleted);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestRefCounter);
