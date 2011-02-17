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
#include <fstream>
#include <sys/utsname.h>

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

  virtual int32_t test1_mss(const std::map< std::string, std::string >& mapin)
  {
    return 42;
  }

  virtual int32_t test1_msas(const std::map< std::string, std::vector< std::string > >& mapin)
  {
    return 42;
  }

  virtual int32_t test1_msv(const std::map< std::string, Variant >& mapin)
  {
    return 42;
  }

  virtual int32_t test1_mis(const std::map< int32_t, std::string >& mapin)
  {
    return 42;
  }

  virtual int32_t test1_miv(const std::map< int32_t, Variant >& mapin)
  {
    return 42;
  }

  virtual int32_t test2_mss(const std::map< std::string, std::string >& mapin)
  {
    return 42;
  }

  virtual int32_t test2_msas(const std::map< std::string, std::vector< std::string > >& mapin)
  {
    return 42;
  }

  virtual int32_t test2_msv(const std::map< std::string, ::DBus::Variant >& mapin)
  {
    return 42;
  }

  virtual int32_t test2_mis(const std::map< int32_t, std::string >& mapin)
  {
    return 42;
  }

  virtual int32_t test2_miv(const std::map< int32_t, ::DBus::Variant >& mapin)
  {
    return 42;
  }

  virtual int32_t test3_mss(const std::map< std::string, std::string >& mapin)
  {
    return 42;
  }

  virtual int32_t test3_msas(const std::map< std::string, std::vector< std::string > >& mapin)
  {
    return 42;
  }

  virtual int32_t test3_msv(const std::map< std::string, ::DBus::Variant >& mapin)
  {
    return 42;
  }

  virtual int32_t test3_mis(const std::map< int32_t, std::string >& mapin)
  {
    return 42;
  }

  virtual int32_t test3_miv(const std::map< int32_t, ::DBus::Variant >& mapin)
  {
    return 42;
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

void *dispatcher_thread(void *) {
  DBus::default_dispatcher->enter();
  return NULL;
}

#define PLOT_FILE "variant_dbus_bench.plot"
#define DAT_FILE "variant_dbus_bench.dat"
#define PNG_FILE "variant_dbus_bench.png"
#define SVG_FILE "variant_dbus_bench.svg"

class BenchVariantDBus : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BenchVariantDBus);
  CPPUNIT_TEST(test1);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {
    m_loop = 300;
    m_map_size = 100;
    struct utsname un;

    CPPUNIT_ASSERT(uname(&un) == 0);

    m_plot.open(PLOT_FILE, ios_base::out);
    CPPUNIT_ASSERT(m_plot);
    m_plot << "set terminal svg enhanced font \"cour,8\" dynamic" << endl;
    m_plot << "set output '" << SVG_FILE << "'" << endl;
    //m_plot << "set terminal png nocrop enhanced font cour 8 size 800,600" << endl;
    //m_plot << "set output '" << PNG_FILE << "'" << endl;
    m_plot << "set title 'metadata serialization over DBus'" << endl;
    m_plot << "set datafile missing '-'" << endl;
    m_plot << "set bmargin 5" << endl;
    m_plot << "set style data histogram" << endl;
    m_plot << "set style histogram cluster gap 1" << endl;
    m_plot << "set yrange [0:]" << endl;
    m_plot << "set style fill solid border -1" << endl;
    m_plot << "set label \"" << un.sysname << " " << un.nodename << " " <<
        un.release << " " << un.version << " " << un.machine <<
        "\" at character 2, 1 noenhanced" << endl;
    m_plot << "plot '" << DAT_FILE << "' ";
    m_plot << "using 2:xtic(1) title 'map<str,str>', ";
    m_plot << "'' using 3 title 'map<str,vect<str>>', ";
    m_plot << "'' using 4 title 'map<str,var>', ";
    m_plot << "'' using 5 title 'map<int,str>', ";
    m_plot << "'' using 6 title 'map<int,var>'" << endl;
    m_plot.close();

    m_pid = fork();

    if (m_pid != 0) {
      DBus::default_dispatcher = new DBus::BusDispatcher(); //m_dispatcher;

      DBus::Connection dbus_connection = DBus::Connection::SessionBus();

      while (!dbus_connection.has_name("variant.dbus.bench_proxy")) {
        sleep(1);
      }

      // wyplay multithreaded dbus-c++ specific
      //CPPUNIT_ASSERT(pthread_create(&m_tid, NULL, &dispatcher_thread, NULL) == 0);
      m_plot.open(DAT_FILE, ios_base::out | ios_base::app);
      CPPUNIT_ASSERT(m_plot);
    }
    else {
      DBus::BusDispatcher m_dispatcher;
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
      m_plot.close();
      //DBus::default_dispatcher->leave();
      //CPPUNIT_ASSERT(pthread_join(m_tid, NULL) == 0);
      delete DBus::default_dispatcher;

      kill(m_pid, SIGTERM);
      waitpid(m_pid, &status, 0);
      CPPUNIT_ASSERT(WIFEXITED(status));
      CPPUNIT_ASSERT(WEXITSTATUS(status) == 0);
    }
  }

  void test1() {
    DBus::Connection conn = DBus::Connection::SessionBus();
    VariantProxy prox(conn);
    Chrono c;

    m_plot.close();
    m_plot.open(DAT_FILE, ios_base::out); //clear content
    CPPUNIT_ASSERT(m_plot);

    m_plot << "\"simple <string, string>\\n data transfer x" << m_loop << "\" ";

    map<string, string> map_ss;
    map<string, vector<string> > map_sas;
    map<int, string> map_is;
    map<string, Variant> map_sv;
    map<int, Variant> map_iv;

    cout << "Building metadata maps" << endl;
    for (unsigned int i = 0; i < m_map_size; ++i) {
      ostringstream oss_key, oss_value;
      oss_value << "mymetadata" << i;
      oss_key << "mymetadata" << i;

      map_ss[oss_key.str()] = oss_value.str();
      map_sas[oss_key.str()].assign(1, oss_value.str());
      map_is[i] = oss_value.str();
      map_sv[oss_key.str()] = oss_value.str();
      map_iv[i] = oss_value.str();
    }

    // make a first dbus call (wyplay thread pool)
    CPPUNIT_ASSERT_EQUAL(42, prox.test1_mss(map_ss));

    cout << "Starting test1_mss" << endl;
    c.start();
    for (unsigned int i = 0; i < m_loop; ++i) {
      CPPUNIT_ASSERT_EQUAL(42, prox.test1_mss(map_ss));
    }
    c.stop();
    m_plot << c << " ";

    cout << "Starting test1_msas" << endl;
    c.start();
    for (unsigned int i = 0; i < m_loop; ++i) {
      CPPUNIT_ASSERT_EQUAL(42, prox.test1_msas(map_sas));
    }
    c.stop();
    m_plot << c << " ";

    cout << "Starting test1_msv" << endl;
    c.start();
    for (unsigned int i = 0; i < m_loop; ++i) {
      CPPUNIT_ASSERT_EQUAL(42, prox.test1_msv(map_sv));
    }
    c.stop();
    m_plot << c << " ";

    cout << "Starting test1_mis" << endl;
    c.start();
    for (unsigned int i = 0; i < m_loop; ++i) {
      CPPUNIT_ASSERT_EQUAL(42, prox.test1_mis(map_is));
    }
    c.stop();
    m_plot << c << " ";

    cout << "Starting test1_miv" << endl;
    c.start();
    for (unsigned int i = 0; i < m_loop; ++i) {
      CPPUNIT_ASSERT_EQUAL(42, prox.test1_miv(map_iv));
    }
    c.stop();
    m_plot << c << " ";


    m_plot << endl;
  }

protected:
  pid_t m_pid;
  pthread_t m_tid;
  uint32_t m_loop, m_map_size;
  fstream m_plot;
};

CPPUNIT_TEST_SUITE_REGISTRATION(BenchVariantDBus);

