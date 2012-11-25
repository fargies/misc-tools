/*
** Copyright (C) 2012 Fargier Sylvain <fargier.sylvain@free.fr>
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
** dbg_test.cc
**
**        Created on: Apr 17, 2012
**   Original Author: fargie_s
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <map>
#include <list>
#include <vector>

#include "dbg.hh"

     #define xstr(s) str(s)
     #define str(s) #s
class TestDbg : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestDbg);
  CPPUNIT_TEST(simple);
  CPPUNIT_TEST_SUITE_END();

public:
  void simple()
  {
      DBG << static_cast<int>(1234);
      DBG << static_cast<int>(-1234);
      DBG << static_cast<unsigned int>(1234);
      DBG << static_cast<short>(1234);
      DBG << "this is a char *";
      DBG << 'c';
      DBG << true << " != " << false;
      DBG << '0' << " < " << '1';
      DBG << " < -- > " << xstr(sizeof (int));
      DBG << " < -- > " << str(sizeof (int));

#ifdef DEBUG_STL
      std::map<int, std::string> m;

      DBG << "dumping empty map: " << m;

      m[0] = "test";
      DBG << "dumping small map: " << m;

      m[1] = "test2";
      DBG << "dumping map: " << m;

      std::list<short> l;

      DBG << "dumping empty list: " << l;

      l.push_back(42);
      DBG << "dumping small list: " << l;

      l.push_back(44);
      DBG << "dumping list: " << l;
#endif
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestDbg);

