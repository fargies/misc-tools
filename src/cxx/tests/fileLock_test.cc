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
** fileLock_test.cc
**
**        Created on: Sat Feb  5 16:21:14 2011
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <signal.h>

#include "fileLock.hh"
#include "sem.hh"

class TestFileLock : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestFileLock);
  CPPUNIT_TEST(test_lock);
  CPPUNIT_TEST(test_fork_lock);
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
    FileLock fl(m_fd);

    CPPUNIT_ASSERT_EQUAL(0, fl.lock(FileLock::FL_RD));
    CPPUNIT_ASSERT_EQUAL(0, fl.unlock());

    CPPUNIT_ASSERT_EQUAL(0, fl.lock(FileLock::FL_WR));
    CPPUNIT_ASSERT_EQUAL(0, fl.unlock());

    CPPUNIT_ASSERT_EQUAL(0, fl.lock(FileLock::FL_RD));
    CPPUNIT_ASSERT_EQUAL(0, fl.lock(FileLock::FL_WR));
    CPPUNIT_ASSERT_EQUAL(0, fl.unlock());
  }

  void test_fork_lock() {
    NamedSem::unlink("test_fork_lock");
    NamedSem sem("test_fork_lock", 0);
    FileLock fl(m_fd);

    CPPUNIT_ASSERT_EQUAL(0, fl.lock(FileLock::FL_RD, false));

    pid_t pid = fork();

    if (pid) {
      CPPUNIT_ASSERT_EQUAL(0, sem.timedwait(1000));
      CPPUNIT_ASSERT(fl.lock(FileLock::FL_WR, false) != 0);
      CPPUNIT_ASSERT_EQUAL(0, fl.unlock());

      CPPUNIT_ASSERT_EQUAL(0, sem.timedwait(1000));
      CPPUNIT_ASSERT(fl.lock(FileLock::FL_RD, false) != 0);
      CPPUNIT_ASSERT(fl.lock(FileLock::FL_WR, false) != 0);

      kill(pid, SIGTERM);
      waitpid(pid, NULL, 0);

      CPPUNIT_ASSERT_EQUAL(0, fl.lock(FileLock::FL_WR));
      sem.unlink();
    }
    else {
      CPPUNIT_ASSERT_EQUAL(0, fl.lock(FileLock::FL_RD, false));
      sem.post();

      CPPUNIT_ASSERT_EQUAL(0, fl.lock(FileLock::FL_WR));
      sem.post();

      sleep(300); // will be killed by the other process
      exit(0);
    }
  }

protected:
  int m_fd;
  std::string m_file;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestFileLock);

