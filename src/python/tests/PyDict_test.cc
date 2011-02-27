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
** PyDict_test.cc
**
**        Created on: Feb 7, 2011
**            Author: fargie_s
**
*/

#include <Python.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include <string>

#include "PyDict.hh"

using namespace std;

class TestPyDict : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestPyDict);
  CPPUNIT_TEST(test_iter);
  CPPUNIT_TEST(test_create);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {
    Py_Initialize();
  }

  void tearDown() {
    Py_Finalize();
  }

  void test_iter() {
    PyObject *obj = PyRun_String("{ 'textKey' : \"textValue\", 1 : 2, 3 : 'textValue2' }", Py_eval_input, Py_None, Py_None);

    CPPUNIT_ASSERT(obj != NULL);

    { /* this bracket is required to assert on refcount */
      PyDict dict(obj);

      PyDict::iterator it = dict.begin();

      CPPUNIT_ASSERT(it != dict.end());
      CPPUNIT_ASSERT_EQUAL(string("1"), (string) it->first);
      CPPUNIT_ASSERT("2" != it->second);
      CPPUNIT_ASSERT(2 == it->second);

      CPPUNIT_ASSERT_EQUAL(string("3"), (string) (++it)->first);
      CPPUNIT_ASSERT("textValue2" == (it++)->second);
      CPPUNIT_ASSERT("textValue" == (it++)->second);
      CPPUNIT_ASSERT(dict.end() == it);
    }

    CPPUNIT_ASSERT_EQUAL((Py_ssize_t) 1, obj->ob_refcnt);
    Py_DECREF(obj);
  }

  void test_create() {
    PyDict dict;

    CPPUNIT_ASSERT(dict.object() != NULL);
    CPPUNIT_ASSERT_EQUAL((Py_ssize_t) 1, dict.object()->ob_refcnt);

    CPPUNIT_ASSERT(dict.empty());
    CPPUNIT_ASSERT_EQUAL((size_t) 0, dict.size());

    dict["test"];
    CPPUNIT_ASSERT_EQUAL((size_t) 1, dict.size());
    CPPUNIT_ASSERT(dict["test"] == Py_None);

    dict["test"] = 2;
    CPPUNIT_ASSERT_EQUAL((size_t) 1, dict.size());
    CPPUNIT_ASSERT(dict["test"] == 2);

    dict["test"] = "string";
    CPPUNIT_ASSERT_EQUAL((size_t) 1, dict.size());
    CPPUNIT_ASSERT(dict["test"] == "string");
    CPPUNIT_ASSERT_EQUAL((Py_ssize_t) 1, dict.object()->ob_refcnt);

    dict.erase("test");
    CPPUNIT_ASSERT(dict.empty());
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPyDict);

