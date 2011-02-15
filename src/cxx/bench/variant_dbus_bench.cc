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
** variant_dbus_bench.cc
**
**        Created on: Feb 14, 2011
**            Author: fargie_s
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <dbus-c++/dbus.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <stdint.h>
//#include <sched.h>

#include "chrono.hh"
#include "Variant.hh"
#include "variant_dbus_adaptor.hh"
#include "variant_dbus_proxy.hh"

using namespace std;

class VariantProxy :
  public variant::dbus::bench_proxy,
  public DBus::ObjectProxy
{
public:
  VariantProxy(DBus::Connection &conn) :
    variant::dbus::bench_proxy(), ObjectProxy(conn, "/", "variant.dbus.bench_proxy")
  {
  }

  int32_t test_v2(const Variant& mapin)
  {
      ::DBus::CallMessage call;
      ::DBus::MessageIter wi = call.writer();

      wi << mapin;
      call.member("test_v");
      ::DBus::Message ret = invoke_method (call);
      ::DBus::MessageIter ri = ret.reader();

      int32_t argout;
      ri >> argout;
      return argout;
  }
};

class VariantAdaptor :
  public variant::dbus::bench_adaptor,
  public DBus::ObjectAdaptor
{
public:
  VariantAdaptor(DBus::Connection &conn) :
    variant::dbus::bench_adaptor(), ObjectAdaptor(conn, "/")
  {
  }

  virtual int32_t test_ass(const map< string, string >& mapin)
  {
    return 42;
  }

  virtual int32_t test_v(const Variant& mapin)
  {
    return 42;
  }

  virtual int32_t test_asv(const map< string, ::DBus::Variant >& mapin)
  {
    return 42;
  }
};

void adaptor_sighandler(int sig)
{
  cerr << "The adaptor received a signal(" << sig << ")" << endl;
  DBus::default_dispatcher->leave();
}

class BenchVariantDBus : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BenchVariantDBus);
  CPPUNIT_TEST(test_variant_dbus);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {
    m_loop = 1000;

    /*struct sched_param param;
    param.sched_priority = 1;
    CPPUNIT_ASSERT_EQUAL(0, sched_setscheduler(0, SCHED_FIFO, &param));*/

    m_pid = fork();

    if (m_pid != 0) {
      DBus::default_dispatcher = & m_dispatcher;

      DBus::Connection dbus_connection = DBus::Connection::SessionBus();

      while (!dbus_connection.has_name("variant.dbus.bench_proxy"))
        sleep(1);
    }
    else {
      DBus::default_dispatcher = & m_dispatcher;

      DBus::Connection dbus_connection = DBus::Connection::SessionBus();

      struct sigaction act;
      memset(&act, 0, sizeof (struct sigaction));
      act.sa_handler = adaptor_sighandler;
      CPPUNIT_ASSERT_EQUAL(0, sigaction(SIGTERM, &act, NULL));

      VariantAdaptor adapt(dbus_connection);
      dbus_connection.request_name("variant.dbus.bench_proxy");
      DBus::default_dispatcher->enter();
      exit(0);
    }
  }

  void tearDown() {
    if (m_pid != 0) {
      int status;

      kill(m_pid, SIGTERM);
      waitpid(m_pid, &status, 0);
      CPPUNIT_ASSERT(WIFEXITED(status));
      CPPUNIT_ASSERT(WEXITSTATUS(status) == 0);
    }
  }

  void test_variant_dbus() {
    DBus::Connection conn = DBus::Connection::SessionBus();
    VariantProxy prox(conn);
    Chrono c;

    map<string, string> imap;
    for (unsigned int i = 0; i < 100; ++i) {
      ostringstream oss_key, oss_value;
      oss_value << "mymetadata" << i;
      oss_key << "mymetadata" << i;

      imap[oss_key.str()] = oss_value.str();
    }

    c.start();
    for (unsigned int i = 0; i < m_loop; ++i) {
      CPPUNIT_ASSERT_EQUAL(42, prox.test_ass(imap));
    }
    c.stop();
    cout << c << endl;

    map<string, Variant> vmap;
    for (map<string, string>::const_iterator it = imap.begin();
         it != imap.end(); ++it) {
      vmap[it->first] = it->second;
    }
    Variant v(vmap);

    ::DBus::Variant dbus_v;
    v >> dbus_v;
    cout << dbus_v.signature() << endl;
    c.start();
    for (unsigned int i = 0; i < m_loop; ++i) {
      CPPUNIT_ASSERT_EQUAL(42, prox.test_v2(v));
    }
    c.stop();
    cout << c << endl;
  }

protected:
  pid_t m_pid;
  DBus::BusDispatcher m_dispatcher;
  uint32_t m_loop;
};

CPPUNIT_TEST_SUITE_REGISTRATION(BenchVariantDBus);
