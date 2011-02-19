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

#include "expat++.hh"

template < class Parser >
void __startHandler(void *data, const XML_Char *name, const XML_Char **attrs)
{
  Expat<Parser> *self = static_cast<Expat<Parser> *> (data);
  if (self->startHandler != NULL)
    (self->*startHandler)(name, attrs);
}

template <class Parser>
Expat<Parser>::Expat() :
  startHandler(NULL), endHandler(NULL)
{
}

#define EXPAT_BUFF_SIZE 2048

template <class Parser>
int Expat<Parser>::parseFile(const char *name)
{
  XML_Parser parser = XML_ParserCreate(NULL);
  FILE *file = fopen(name, "r");

  XML_SetUserData(parser, this);
  XML_SetStartElementHandler(parser, &(__startHandler<Parser>));

  while (true) {
    int bytes_read;
    void *buff = XML_GetBuffer(parser, EXPAT_BUFF_SIZE);
    if (buff == NULL) {
      /* handle error */
    }

    bytes_read = fread(buff, EXPAT_BUFF_SIZE, 1, file);
    if (bytes_read < 0) {
      /* handle error */
    }

    if (! XML_ParseBuffer(parser, bytes_read, bytes_read == 0)) {
      /* handle parse error */
    }

    if (bytes_read == 0)
      break;
  }

  fclose(file);
  XML_ParserFree(parser);
}


#endif /* EXPAT_HXX_ */
