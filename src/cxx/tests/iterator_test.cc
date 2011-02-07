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
** iterator_test.cc
**
**        Created on: Fri 04 Feb 2011 01:26:11 AM CET
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <unistd.h>

#include "iterator.hh"

#include <iostream>
#include <vector>
#include <string>

class TestIterator : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestIterator);
  CPPUNIT_TEST(test_list);
  CPPUNIT_TEST_SUITE_END();

public:
  void test_list() {
    std::list< int > orig_list, check_list;

    orig_list.push_back(1);
    orig_list.push_back(2);
    orig_list.push_back(3);

//    ListIterator< int > iter(orig_list);
    STLIter< int >::List iter(orig_list);

    for (; iter; ++iter) {
      check_list.push_back(*iter);
    }
    CPPUNIT_ASSERT(check_list == orig_list);

    check_list.clear();
//    STLIter< int >::List stliter(orig_list);
    std::vector< int > vec;
    STLIter< int >::Vector stliter(vec);
    std::map< int, std::string > map;
    STLIter< int, std::string >::Map stliter_map(map);
//    ListIterator< int > 
//    STLIter::List< int >
  }
 };

CPPUNIT_TEST_SUITE_REGISTRATION(TestIterator);

