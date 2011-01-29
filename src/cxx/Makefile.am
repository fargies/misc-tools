
noinst_LTLIBRARIES = libtest.la

check_PROGRAMS = test_chrono_t

libtest_la_SOURCES = test_main.cc
libtest_la_LIBADD = $(CPPUNIT_LIBS)
libtest_la_CXXFLAGS = $(CPPUNIT_CFLAGS)

test_chrono_t_SOURCES = chrono.hh chrono.cc chrono_test.cc
test_chrono_t_LDADD = $(CPPUNIT_LIBS) $(builddir)/libtest.la -lrt
test_chrono_t_CXXFLAGS = $(CPPUNIT_CFLAGS)

TESTS = test_chrono_t
