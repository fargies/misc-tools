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
** fileLock_bench.cc
**
**        Created on: Sat Feb  5 19:11:03 2011
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "chrono.hh"
#include "fileLock.hh"

using namespace std;

class BenchFileLock : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BenchFileLock);
  CPPUNIT_TEST(test_lock);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {
    char file[] = "/tmp/testfile.XXXXXX";

    m_fd = mkstemp(file);
    CPPUNIT_ASSERT(m_fd >= 0);
    m_file = file;
  }

  void tearDown() {
    close(m_fd);
    CPPUNIT_ASSERT_EQUAL(0, unlink(m_file.c_str()));
  }

  void test_lock() {
    Chrono c;
    uint32_t i;

    cout << "\nStarting create/lock/unlock benchmark" << endl; 
    c.start();
    for (i = 0; i < 10000; ++i) {
      FileLock l(m_fd);
      CPPUNIT_ASSERT_EQUAL(0, l.lock(FileLock::FL_WR));
      CPPUNIT_ASSERT_EQUAL(0, l.unlock());
    }
    c.stop();

    struct timespec t;
    c.getTimespec(t);
    cout << "\t" << t.tv_sec * 100000 + t.tv_nsec / 10000 << " nsec/FileLock " << endl;

    cout << "\t" << (10000 * 1000) / c.getMiliSecs() << " FileLock/sec" << endl;
  }

protected:
  int m_fd;
  string m_file;
};

CPPUNIT_TEST_SUITE_REGISTRATION(BenchFileLock);


