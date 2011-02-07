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
** sem_test.cc
**
**        Created on: Feb 5, 2011
**            Author: fargie_s
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "sem.hh"

#include <iostream>

class TestSem : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestSem);
  CPPUNIT_TEST(test_sem);
  CPPUNIT_TEST(test_pshared_sem);
  CPPUNIT_TEST(test_named_sem);
  CPPUNIT_TEST_SUITE_END();

public:
  void test_sem() {
  	Sem s;

  	CPPUNIT_ASSERT_EQUAL(1, s.getValue());
  	CPPUNIT_ASSERT_EQUAL(0, s.trywait());

  	CPPUNIT_ASSERT_EQUAL(0, s.getValue());
  	CPPUNIT_ASSERT_EQUAL(EAGAIN, s.trywait());
  	CPPUNIT_ASSERT_EQUAL(ETIMEDOUT, s.timedwait(10));

  	CPPUNIT_ASSERT_EQUAL(0, s.post());
  	CPPUNIT_ASSERT_EQUAL(1, s.getValue());
  	CPPUNIT_ASSERT_EQUAL(0, s.timedwait(10));
  }

  void test_pshared_sem() {
    Sem s(1);

    CPPUNIT_ASSERT_EQUAL(0, s.timedwait(10));
    CPPUNIT_ASSERT_EQUAL(0, s.getValue());

    pid_t pid = fork();

    if (pid) {
      int status;

      CPPUNIT_ASSERT_EQUAL(0, s.timedwait(1000));

      waitpid(pid, &status, 0);
      CPPUNIT_ASSERT(WIFEXITED(status));
      CPPUNIT_ASSERT_EQUAL(0, WEXITSTATUS(status));
    }
    else {
      CPPUNIT_ASSERT_EQUAL(0, s.post());
      _exit(0);
    }
  }

  void test_named_sem() {
    NamedSem::unlink("mySemTest");

    NamedSem s("mySemTest");

    CPPUNIT_ASSERT_EQUAL(0, s.trywait());
    CPPUNIT_ASSERT_EQUAL(0, s.getValue());

    pid_t pid = fork();

    if (pid) {
      int status;

      CPPUNIT_ASSERT_EQUAL(0, s.timedwait(1000));

      waitpid(pid, &status, 0);
      CPPUNIT_ASSERT(WIFEXITED(status));
      CPPUNIT_ASSERT_EQUAL(0, WEXITSTATUS(status));
      s.unlink();
    }
    else {
      NamedSem s2("mySemTest");

      CPPUNIT_ASSERT_EQUAL(s.getName(), s2.getName());
      CPPUNIT_ASSERT_EQUAL(0, s2.post());
      _exit(0);
    }
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSem);
