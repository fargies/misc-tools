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

using namespace std;

class TestPyList : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestPyList);
  CPPUNIT_TEST(test_create);
  CPPUNIT_TEST(test_insert);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_erase);
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
/*    PyList dict1, dict2;

    dict2["test"] = 42;

    dict1 = dict2;
    CPPUNIT_ASSERT(dict1.count("test") == 1);
    CPPUNIT_ASSERT(dict1 == dict2);

    dict2["test2"] = 43;
    CPPUNIT_ASSERT(dict1.count("test2") == 0);
    CPPUNIT_ASSERT(dict1 != dict2);

    dict1["test2"] = dict2["test2"];
    CPPUNIT_ASSERT(dict1 == dict2);

    dict1["test3"] = dict2["test3"] = 44;
    CPPUNIT_ASSERT(dict1 == dict2);*/
  }

  void test_erase() {
/*    PyList dict;
    dict["test1"] = dict["test2"] = "test";

    dict.erase(dict.find("test2"));
    CPPUNIT_ASSERT(dict.size() == 1);

    CPPUNIT_ASSERT(dict.erase("test1") == 1);
    CPPUNIT_ASSERT(dict.erase("test1") == 0);
    CPPUNIT_ASSERT(dict.empty());

    for (int i = 0; i < 100; ++i)
      dict[i] = i;

    PyList::iterator er_begin = dict.begin();

    for (int i = 0; i < 10; ++i)
      ++er_begin;

    PyList::iterator er_end = er_begin;

    for (int i = 0; i < 20; ++i)
      ++er_end;

    dict.erase(er_begin, er_end);
    CPPUNIT_ASSERT(dict.size() == 80);*/
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPyList);



