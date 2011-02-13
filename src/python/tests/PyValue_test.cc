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
** PyValue_test.cc
**
**        Created on: Feb 13, 2011
**            Author: fargie_s
**
*/

#include <Python.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <string>

#include "PyValue.hh"

using namespace std;

class TestPyValue : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestPyValue);
  CPPUNIT_TEST(test_value);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {
    Py_Initialize();
  }

  void tearDown() {
    Py_Finalize();
  }

  void test_value() {
    PyValue val, val2;

    CPPUNIT_ASSERT(val == val2);
    CPPUNIT_ASSERT(val == Py_None);
    CPPUNIT_ASSERT(Py_None == val);

    val = 2;
    CPPUNIT_ASSERT(val == 2);
    CPPUNIT_ASSERT(2 == val);
    CPPUNIT_ASSERT("string" != val);
    CPPUNIT_ASSERT(val != "string");
    CPPUNIT_ASSERT(val != val2);
    int i = val;
    CPPUNIT_ASSERT(i == 2);
    CPPUNIT_ASSERT_EQUAL(2, (int) val);

    val = "test";
    CPPUNIT_ASSERT(val == "test");
    CPPUNIT_ASSERT("test" == val);
    CPPUNIT_ASSERT(string("test") == val);
    CPPUNIT_ASSERT(val == string("test"));
    CPPUNIT_ASSERT(val != 2);
    CPPUNIT_ASSERT(2 != val);
    CPPUNIT_ASSERT(val != val2);
    string test = val;
    const char *ctest = val.c_str();
    val = test;
    CPPUNIT_ASSERT(test == "test");
    CPPUNIT_ASSERT_EQUAL(string("test"), (string) val);

    val2 = val;
    CPPUNIT_ASSERT(val2 == val);
    CPPUNIT_ASSERT(val2 == "test");
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPyValue);
