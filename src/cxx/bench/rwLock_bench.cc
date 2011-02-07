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
** rwLock_bench.cc
**
**        Created on: Sat Feb  5 17:55:12 2011
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>

#include "rwLock.hh"
#include "chrono.hh"

using namespace std;

class BenchRWLock : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BenchRWLock);
  CPPUNIT_TEST(test_create);
  CPPUNIT_TEST(test_rlock);
  CPPUNIT_TEST(test_wlock);
  CPPUNIT_TEST(test_read_create);
  CPPUNIT_TEST_SUITE_END();

public:
  void test_create() {
    Chrono c;
    uint32_t i;

    cout << "\nStarting Creation benchmark" << endl; 
    c.start();
    for (i = 0; i < 10000; ++i) {
      RWLock l("myLock");
    }
    c.stop();

    struct timespec t;
    c.getTimespec(t);
    cout << "\t" << t.tv_sec * 100000 + t.tv_nsec / 10000 << " nsec/RWLock " << endl;

    cout << "\t" << (10000 * 1000) / c.getMiliSecs() << " RWLock/sec" << endl;
  }

  void test_rlock() {
    Chrono c;
    uint32_t i;

    cout << "\nStarting read lock benchmark" << endl; 
    RWLock l("myLock");
    c.start();
    for (i = 0; i < 10000; ++i) {
        CPPUNIT_ASSERT_EQUAL(0, l.lock(RWLock::RWL_READ));
        l.unlock();
    }
    c.stop();

    struct timespec t;
    c.getTimespec(t);
    cout << "\t" << t.tv_sec * 100000 + t.tv_nsec / 10000 << " nsec/RWLock " << endl;

    cout << "\t" << (10000 * 1000) / c.getMiliSecs() << " RWLock/sec" << endl;
  }

  void test_wlock() {
    Chrono c;
    uint32_t i;

    cout << "\nStarting write lock benchmark" << endl; 
    RWLock l("myLock");
    c.start();
    for (i = 0; i < 10000; ++i) {
        CPPUNIT_ASSERT_EQUAL(0, l.lock(RWLock::RWL_WRITE));
        l.unlock();
    }
    c.stop();

    struct timespec t;
    c.getTimespec(t);
    cout << "\t" << t.tv_sec * 100000 + t.tv_nsec / 10000 << " nsec/RWLock " << endl;

    cout << "\t" << (10000 * 1000) / c.getMiliSecs() << " RWLock/sec" << endl;
  }

  void test_read_create() {
    Chrono c;
    uint32_t i;

    cout << "\nStarting create and read lock benchmark" << endl; 
    c.start();
    for (i = 0; i < 10000; ++i) {
        RWLock l("myLock");
        CPPUNIT_ASSERT_EQUAL(0, l.lock(RWLock::RWL_WRITE));
        l.unlock();
    }
    c.stop();

    struct timespec t;
    c.getTimespec(t);
    cout << "\t" << t.tv_sec * 100000 + t.tv_nsec / 10000 << " nsec/RWLock " << endl;

    cout << "\t" << (10000 * 1000) / c.getMiliSecs() << " RWLock/sec" << endl;
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(BenchRWLock);

