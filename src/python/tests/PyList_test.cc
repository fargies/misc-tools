/*
** Copyright (C) 2011 Fargier Sylvain <fargier.sylvain@free.fr>
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
** PyList_test.cc
**
**        Created on: Mar 17, 2011
**   Orignial Author: fargie_s
**
*/

#include <Python.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include <string>

#include "PyList.hh"
#include "PyDict.hh"

using namespace std;

class TestPyList : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestPyList);
  CPPUNIT_TEST(test_create);
  CPPUNIT_TEST(test_insert);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_map);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {
    Py_Initialize();
  }

  void tearDown() {
    Py_Finalize();
  }

  void test_create() {
    PyList list;

    CPPUNIT_ASSERT(list.object() != NULL);
    CPPUNIT_ASSERT_EQUAL((Py_ssize_t) 1, list.object()->ob_refcnt);

    CPPUNIT_ASSERT(list.empty());
    CPPUNIT_ASSERT_EQUAL((size_t) 0, list.size());

    list.push_front("test");
    CPPUNIT_ASSERT_EQUAL((size_t) 1, list.size());
    CPPUNIT_ASSERT(list.front() == "test");

    list.push_back("test2");
    CPPUNIT_ASSERT_EQUAL((size_t) 2, list.size());
    CPPUNIT_ASSERT(list.back() == "test2");

    list.front() = "newtest";
    CPPUNIT_ASSERT(list.front() == "newtest");

    list.erase(list.begin(), list.end());
    CPPUNIT_ASSERT(list.empty());
  }

  void test_insert() {
    PyList list;

    PyList::iterator it = list.insert(list.begin(), "42");
    CPPUNIT_ASSERT(it != list.end());
    CPPUNIT_ASSERT(*it == "42");
    CPPUNIT_ASSERT(list.front() == "42");

    list.insert(list.end(), 2, "44");
    CPPUNIT_ASSERT_EQUAL((PyList::size_type) 3, list.size());
    CPPUNIT_ASSERT(list.front() == "42");
    CPPUNIT_ASSERT(list.back() == "44");

    list.insert(list.begin(), 40);
    CPPUNIT_ASSERT_EQUAL((PyList::size_type) 4, list.size());
    CPPUNIT_ASSERT(list.front() == 40);

    list.clear();
    CPPUNIT_ASSERT(list.empty());
  }

  void test_copy() {
/*    PyList list1, list2;

    list2.push_back(42);

    list1 = list2;
    CPPUNIT_ASSERT(list1.size() == 1);
    CPPUNIT_ASSERT(list1 == list2);

    list2.push_back(43);
    CPPUNIT_ASSERT(list1.size() == 1);
    CPPUNIT_ASSERT(list1 != list2);

    list1.push_back(43);
    CPPUNIT_ASSERT(list1 == list2);

    list1.front() = list2.front() = 72;
    CPPUNIT_ASSERT(list1 == list2);*/
  }

  void test_map() {
    typedef PyListBase<PyDict> PyDictList;
    PyDictList list;

    list.push_back(PyDictList::value_type());

    CPPUNIT_ASSERT_EQUAL((PyDictList::size_type) 1, list.size());

    list.front()["test"] = 42;

    PyDictList::reference ref = list.front();
    ref["test2"] = "test2_value";

    CPPUNIT_ASSERT_EQUAL((PyDict::size_type) 2, list.front().size());

    list.push_front(list.front());
    CPPUNIT_ASSERT(list.size() == 2);
    list.front()["test3"] = "test3_value";

    CPPUNIT_ASSERT_EQUAL((PyDict::size_type) 2, list.back().size());
    for (PyDictList::iterator it = list.begin();
         it != list.end(); ++it)
      ;
    list.clear();
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPyList);

