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
** PySeq_test.cc
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

#include "PySeq.hh"

using namespace std;

class TestPySeq : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestPySeq);
  CPPUNIT_TEST(test_iter);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_erase);
  CPPUNIT_TEST(test_seqvalue);
  CPPUNIT_TEST_SUITE_END();

public:
  TestPySeq() :
    m_globals(NULL)
  {
  }

  void setUp() {
    Py_Initialize();
  }

  void tearDown() {
    if (m_globals) {
      Py_DECREF(m_globals);
      m_globals = NULL;
    }

    Py_Finalize();
  }

protected:
  PyObject *m_globals;

  PyObject *pyrun(const char *str) {
    if (m_globals == NULL) {
      m_globals = PyDict_New();
      PyDict_SetItemString(m_globals, "__builtins__", PyEval_GetBuiltins());
    }
    return PyRun_String(str, Py_eval_input, m_globals, m_globals);
  }

  void test_iter() {
    PyObject *obj = pyrun("( 1, 2, 'textKey', \"5\" )");

    CPPUNIT_ASSERT(obj != NULL);

    { /* this bracket is required to assert on refcount */
      PySeq seq(obj);

      CPPUNIT_ASSERT_EQUAL(1, (int) seq.front());
      CPPUNIT_ASSERT_EQUAL((PySeq::size_type) 4, seq.size());
      CPPUNIT_ASSERT_EQUAL(string("5"), (string) seq.back());

      PySeq::iterator it = seq.begin();

      CPPUNIT_ASSERT(it != seq.end());
      CPPUNIT_ASSERT_EQUAL(1, (int) (*it));

      CPPUNIT_ASSERT_EQUAL(2, (int) (*++it));
      CPPUNIT_ASSERT(2 == (*it++));
      CPPUNIT_ASSERT("textKey" == (*it++));
      CPPUNIT_ASSERT_EQUAL(string("5"), (string) (*it));
      CPPUNIT_ASSERT(seq.end() == ++it);
    }

    CPPUNIT_ASSERT_EQUAL((Py_ssize_t) 1, obj->ob_refcnt);
    Py_DECREF(obj);
  }

  void test_copy() {
/*    PySeq dict1, dict2;

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
    PyObject *obj = pyrun("range(0, 20)");
    PyErr_PrintEx(1);

    CPPUNIT_ASSERT(obj != NULL);
    PySeq seq(obj);
    Py_DECREF(obj);

    CPPUNIT_ASSERT_EQUAL((PySeq::size_type) 20, seq.size());
    CPPUNIT_ASSERT(seq.front() == 0);
    CPPUNIT_ASSERT(seq.back() == 19);

    seq.pop_front();
    CPPUNIT_ASSERT_EQUAL((PySeq::size_type) 19, seq.size());
    CPPUNIT_ASSERT(seq.front() == 1);

    seq.pop_back();
    CPPUNIT_ASSERT_EQUAL((PySeq::size_type) 18, seq.size());
    CPPUNIT_ASSERT(seq.back() == 18);

    PySeq::iterator it_start, it_end;

    it_start = seq.begin();
    for (int i = 0; i < 9; ++i)
      ++it_start;
    CPPUNIT_ASSERT(*it_start == 10);

    it_end = it_start;
    for (int i = 0; i < 5; ++i)
      ++it_end;
    CPPUNIT_ASSERT(*it_end == 15);

    seq.erase(it_start, it_end);
    CPPUNIT_ASSERT_EQUAL((PySeq::size_type) 13, seq.size());

    std::cout << (std::string) seq << std::endl;
    it_start = ++seq.begin();
    CPPUNIT_ASSERT(*it_start == 2);
    std::cout << (std::string) seq << std::endl;
    seq.erase(it_start);
    CPPUNIT_ASSERT_EQUAL((PySeq::size_type) 12, seq.size());

    obj = pyrun("[ 1, ] + range(3, 10) + range(15, 19)");
    CPPUNIT_ASSERT(obj != NULL);
    std::cout << (std::string) seq << std::endl;
    std::cout << (std::string) PyValue(obj) << std::endl;
    CPPUNIT_ASSERT(seq == obj);
    Py_DECREF(obj);
  }

  void test_seqvalue() {
    /* Testing PySeqValues with no associated seq */
    PySeqValue<PyValue> v1;
    PySeqValue<PyValue> v2(v1);

    v1 = v2;
    v1 = PyValue(32);
    v1 = 42;
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestPySeq);

