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
** expat++.h
**
**        Created on: Feb 18, 2011
**            Author: fargie_s
**
*/

#ifndef EXPATPP_H_
#define EXPATPP_H_

#include <expat.h>

template <class Parser>
class Expat
{
public:
  Expat();
  ~Expat();

  /**
   * @return
   * - 0 on success
   * - ENOENT the given file doesn't exist
   */
  int parseFile(const char *file);

  typedef void (Parser::*StartHandler)(const XML_Char *name, const XML_Char **attrs);
  typedef void (Parser::*EndHandler)(const XML_Char *name);

  StartHandler startHandler;
  EndHandler endHandler;

private:
  XML_Parser m_parser;
};

#include "expat++.hxx"

#endif /* EXPAT_H_ */
