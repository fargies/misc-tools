/*
** Copyright (C) 2010 Fargier Sylvain <fargier.sylvain@free.fr>
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
** chrono_test.cc
**
**        Created on: Sep 24, 2010
**            Author: fargie_s
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <unistd.h>

#include "chrono.hh"

#include <iostream>
class TestChrono : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestChrono);
  CPPUNIT_TEST(test_chrono);
  CPPUNIT_TEST_SUITE_END();

public:
  void test_chrono() {
    Chrono c;

    CPPUNIT_ASSERT(c.getMiliSecs() == 0);
    c.start();
    sleep(1);
    c.stop();
    CPPUNIT_ASSERT(c.getMiliSecs() >= 1000);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestChrono);
