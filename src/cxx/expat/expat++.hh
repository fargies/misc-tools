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

#ifndef EXPATPP_HH_
#define EXPATPP_HH_

#include <map>
#include <string>
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
   * - EIO when something went wrong while trying to read the file
   * - EILSEQ when something failed while parsing
   */
  int parseFile(const char *file);

  typedef void (Parser::*Handler)(const std::string &name);
  typedef std::map<std::string, const XML_Char *> Attributes;

  /**
   * @brief accumulate items data
   * @details if accu is NULL, an internal accumulator will be used (m_data)
   * @param accu the data accumulator
   */
  void addDataWatch(std::string *accu = NULL);
  void removeDataWatch();

  void setHandlers(Handler start, Handler end);
  void setStartHandler(Handler start);
  void setEndHandler(Handler end);

  /**
   * @brief returns current item attributes
   * @details must be used only in startHandler
   */
  Attributes &attributes();
private:
  XML_Parser m_parser;
  std::string *m_accu;
  const XML_Char **m_raw_attrs;
  Attributes m_attrs;

protected:
  std::string m_data;

  /**
   * @brief xml item start handler
   */
  Handler start;

  /**
   * @brief xml item end handler
   */
  Handler end;

  template<class P> friend void __startHdlr(void *, const XML_Char *, const XML_Char **);
  template<class P> friend void __endHdlr(void *, const XML_Char *);
  template<class P> friend void __dataHdlr(void *, const XML_Char *, int);
};

#include "expat++.hxx"

#endif /* EXPATPP_HH_ */
