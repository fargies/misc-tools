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
** extref_test.cc
**
**        Created on: Nov 09, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "extRef.hh"

class ExtRefedObject
{
public:
    ExtRefedObject(bool &del) :
        m_del(del)
    {
        m_del = false;
    }

    ~ExtRefedObject()
    {
        m_del = true;
    }

protected:
    bool &m_del;
};

class TestExtRef : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestExtRef);
    CPPUNIT_TEST(simple);
    CPPUNIT_TEST(object);
    CPPUNIT_TEST_SUITE_END();

public:
    void simple()
    {
        ExtRef<int> r1(new int(12));

        CPPUNIT_ASSERT(*r1 == 12);

        ExtRef<int> r2 = r1;
        CPPUNIT_ASSERT(&(*r2) == r1);

        ExtRef<int> r3(r2);
        CPPUNIT_ASSERT(&(*r3) == r1);

        CPPUNIT_ASSERT(r3.getNumRef() == 3);
        r3 = r1;
    }

    void object()
    {
        bool deleted = true;
        {
            ExtRef<ExtRefedObject> r1(new ExtRefedObject(deleted));
            CPPUNIT_ASSERT(deleted == false);
        }
        CPPUNIT_ASSERT(deleted == true);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExtRef);

