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
** locker_test.cc
**
**        Created on: Apr 08, 2012
**   Original Author: fargie_s
**
*/


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "threading/mutex.hh"
#include "threading/locker.hh"

class TestLocker : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestLocker);
    CPPUNIT_TEST(simple);
    CPPUNIT_TEST_SUITE_END();

public:
    void simple()
    {
        Mutex m;

        {
            Locker l(m);

            CPPUNIT_ASSERT(!m.trylock());
        }
        CPPUNIT_ASSERT(m.trylock());
        m.unlock();
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLocker);
