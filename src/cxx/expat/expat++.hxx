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
** expat++.hxx
**
**        Created on: Feb 19, 2011
**            Author: fargie_s
**
*/

#ifndef EXPAT_HXX_
#define EXPAT_HXX_

#include <errno.h>
#include "expat++.hh"

template <class Parser>
void __startHdlr(void *data, const XML_Char *name, const XML_Char **attrs)
{
  Parser *self = static_cast<Parser *> (data);
  if (self->start != NULL) {
    self->m_raw_attrs = attrs;
    self->m_attrs.clear();
    (self->*(self->start))(name);
  }
}

template <class Parser>
void __endHdlr(void *data, const XML_Char *name)
{
  Parser *self = static_cast<Parser *> (data);
  if (self->end != NULL)
    (self->*(self->end))(name);
}

template <class Parser>
void __dataHdlr(void *data, const XML_Char *str, int size)
{
  Parser *self = static_cast<Parser *> (data);
  if (self->m_accu != NULL)
    self->m_accu->append(str, size);
}

template <class Parser>
Expat<Parser>::Expat() :
  start(NULL), end(NULL), m_accu(NULL)
{
  m_parser = XML_ParserCreate(NULL);
  XML_SetUserData(m_parser, this);
  XML_SetStartElementHandler(m_parser, &(__startHdlr<Parser>));
  XML_SetEndElementHandler(m_parser, &(__endHdlr<Parser>));
  XML_SetCharacterDataHandler(m_parser, NULL);
}

template <class Parser>
Expat<Parser>::~Expat()
{
  XML_ParserFree(m_parser);
}

template <class Parser>
void Expat<Parser>::addDataWatch(std::string *accu)
{
  if (accu != NULL)
    m_accu = accu;
  else {
    m_data.clear();
    m_accu = &m_data;
  }
  XML_SetCharacterDataHandler(m_parser, &(__dataHdlr<Parser>));
}

template <class Parser>
void Expat<Parser>::removeDataWatch()
{
  m_accu = NULL;
  XML_SetCharacterDataHandler(m_parser, NULL);
}

template <class Parser>
std::map<std::string, const XML_Char *> &Expat<Parser>::attributes()
{
  if (m_raw_attrs && m_attrs.empty()) {
    while (*m_raw_attrs != NULL) {
      const XML_Char *key = *(m_raw_attrs++);
      m_attrs[key] = *(m_raw_attrs++);
    }
  }
  return m_attrs;
}

#define EXPAT_BUFF_SIZE 2048

template <class Parser>
int Expat<Parser>::parseFile(const char *name)
{
  FILE *file = fopen(name, "r");

  if (file == NULL)
    return ENOENT;

  int bytes_read, ret = 0;
  void *buff;
  while (true) {
    if ((buff = XML_GetBuffer(m_parser, EXPAT_BUFF_SIZE)) == NULL) {
      ret = ENOMEM;
      break;
    }

    bytes_read = fread(buff, 1, EXPAT_BUFF_SIZE, file);
    if (bytes_read < 0) {
      ret = EIO;
      break;
    }

    if (!XML_ParseBuffer(m_parser, bytes_read, bytes_read == 0)) {
      ret = EILSEQ;
      break;
    }

    if (bytes_read == 0)
      break;
  }

  fclose(file);
  return ret;
}

#endif /* EXPAT_HXX_ */

