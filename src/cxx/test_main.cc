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
** test_main.cc
**
**        Created on: Thu 29 Apr 2010 11:18:09 AM CEST
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <iostream>

/* Test program */
int main(int , char** argv)
{
  CppUnit::TestResult testresult;
  CppUnit::TestResultCollector collectedresults;
  CppUnit::TestRunner testrunner;
  std::ofstream fb;

  testresult.addListener(&collectedresults);
  testrunner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
  testrunner.run(testresult);

  fb.open((std::string(argv[0]) + ".xml").c_str());
  CppUnit::XmlOutputter xml_outputter(&collectedresults, fb);
  xml_outputter.write();
  fb.close();

  fb.open((std::string(argv[0]) + ".cmp").c_str());
  CppUnit::CompilerOutputter comp_outputter(&collectedresults, fb);
  comp_outputter.write();
  fb.close();

  CppUnit::TextOutputter txt_outputter(&collectedresults, std::cout);
  txt_outputter.write();

  return collectedresults.wasSuccessful() ? 0 : 1;
}

