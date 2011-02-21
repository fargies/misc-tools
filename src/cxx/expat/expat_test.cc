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
** expat_test.cc
**
**        Created on: Feb 18, 2011
**            Author: fargie_s
**
*/


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "expat++.hh"

class ExpatTester : public Expat<ExpatTester>
{
public:
  ExpatTester()
  {
    start = &ExpatTester::startHandlerTest;
    end = &ExpatTester::endHandlerTest;
  }

  void startHandlerTest(const std::string &name)
  {
    std::cout << "found " << name << " start tag" << std::endl;
    if (name == "item")
      addDataWatch(NULL);

    Attributes &attr = attributes();
    for (Attributes::const_iterator it = attr.begin();
         it != attr.end(); ++it) {
      std::cout << "  attribute: " << it->first << " : " <<
        it->second << std::endl;
    }
  }

  void endHandlerTest(const std::string &name)
  {
    std::cout << "found " << name << " end tag" << std::endl;
    if (name == "item") {
      if (!m_data.empty()) {
        std::cout << "  data: " << m_data << std::endl;
        m_data.clear();
      }
      removeDataWatch();
    }
  }
};

class TestExpat : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestExpat);
  CPPUNIT_TEST(test_parser);
  CPPUNIT_TEST_SUITE_END();

public:
  void test_parser() {
    ExpatTester tester;

    CPPUNIT_ASSERT_EQUAL(0, tester.parseFile("sample.xml"));
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestExpat);

