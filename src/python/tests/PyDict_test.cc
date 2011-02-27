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
  CPPUNIT_TEST(test_insert);
  CPPUNIT_TEST(test_find);
  CPPUNIT_TEST(test_find_iter);
  CPPUNIT_TEST(test_count);
  CPPUNIT_TEST(test_copy);
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
    CPPUNIT_ASSERT_EQUAL(0, dict.size());

    dict["test"];
    CPPUNIT_ASSERT_EQUAL(1, dict.size());
    CPPUNIT_ASSERT(dict["test"] == Py_None);

    dict["test"] = 2;
    CPPUNIT_ASSERT_EQUAL(1, dict.size());
    CPPUNIT_ASSERT(dict["test"] == 2);

    dict["test"] = "string";
    CPPUNIT_ASSERT_EQUAL(1, dict.size());
    CPPUNIT_ASSERT(dict["test"] == "string");
    CPPUNIT_ASSERT_EQUAL((Py_ssize_t) 1, dict.object()->ob_refcnt);

    dict.erase("test");
    CPPUNIT_ASSERT(dict.empty());
  }

  void test_insert() {
    PyDict dict;

    std::pair<PyDict::iterator, bool> res;

    res = dict.insert(make_pair(PyValue("toto"), PyValue("test")));
    CPPUNIT_ASSERT(res.first != dict.end());
    CPPUNIT_ASSERT(res.second == true);
    CPPUNIT_ASSERT(res.first->first == "toto");
    CPPUNIT_ASSERT(res.first->second == "test");

    res = dict.insert(make_pair(PyValue("toto"), PyValue("test2")));
    CPPUNIT_ASSERT(res.first != dict.end());
    CPPUNIT_ASSERT(res.second == false);
    CPPUNIT_ASSERT(res.first->second == "test");
    CPPUNIT_ASSERT(dict["toto"] == "test");

    dict.clear();
    CPPUNIT_ASSERT(dict.empty());

    PyDict::iterator it = dict.insert(dict.begin(),
        make_pair(PyValue("toto"), PyValue("test2")));
    CPPUNIT_ASSERT(it != dict.end());
    CPPUNIT_ASSERT(it->first == "toto");
    CPPUNIT_ASSERT(it->second == "test2");

    it = dict.insert(dict.begin(),
        make_pair(PyValue("toto"), PyValue("test3")));
    CPPUNIT_ASSERT(it != dict.end());
    CPPUNIT_ASSERT(it->second == "test2");
  }

  void test_find() {
    PyDict dict;

    dict["item1"] = "1";
    dict["item2"] = "2";
    dict["item3"] = "3";

    PyDict::iterator it = dict.find("item1");
    CPPUNIT_ASSERT(it != dict.end());
    CPPUNIT_ASSERT(it->first == "item1");
    CPPUNIT_ASSERT(it->second == "1");

    it = dict.find(PyValue("item2"));
    CPPUNIT_ASSERT(it != dict.end());
    CPPUNIT_ASSERT(it->first == "item2");
    CPPUNIT_ASSERT(it->second == "2");

    it = dict.find(PyValue(3));
    CPPUNIT_ASSERT(it == dict.end());
  }

  void test_find_iter() {
    PyDict dict;

    dict["item1"] = "1";
    dict["item2"] = "2";
    dict["item3"] = "3";

    PyDict::iterator it = dict.find("item3");
    CPPUNIT_ASSERT(it != dict.end());
    CPPUNIT_ASSERT(++it != dict.end());
    CPPUNIT_ASSERT(it->first == "item2");
    CPPUNIT_ASSERT((++it)->first == "item3");
    CPPUNIT_ASSERT((++it)->first == "item1");
  }

  void test_count() {
    PyDict dict;

    dict["item1"] = "1";
    dict[2] = 2;

    CPPUNIT_ASSERT_EQUAL(1, dict.count("item1"));
    CPPUNIT_ASSERT_EQUAL(1, dict.count(PyValue("item1")));
    CPPUNIT_ASSERT_EQUAL(0, dict.count("item2"));
    CPPUNIT_ASSERT_EQUAL(1, dict.count(2));
    CPPUNIT_ASSERT_EQUAL(0, dict.count("2"));
  }

  void test_copy() {
    PyDict dict1, dict2;

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
    CPPUNIT_ASSERT(dict1 == dict2);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPyDict);

