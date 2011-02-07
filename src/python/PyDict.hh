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
** PyDict.hh
**
**        Created on: Feb 7, 2011
**            Author: fargie_s
**
*/

#ifndef PYDICT_HH_
#define PYDICT_HH_

#include <iterator>
#include <Python.h>

template < typename T >
class PyDictIterator : public std::iterator< std::input_iterator_tag, T > {
public:
  PyDictIterator(PyObject *obj);
  PyDictIterator(const PyDictIterator &);
  ~PyDictIterator();
  PyDictIterator& operator ++();
  PyDictIterator operator ++(int);
  bool operator ==(const PyDictIterator &rhs);
  bool operator !=(const PyDictIterator &rhs);
  T &operator *();

protected:
  PyObject *m_dict;
  Py_ssize_t m_pos;
  T m_value;
};

/*class PyDict {
public:
  PyDict(PyObject *);
  ~PyDict();
};
*/
#include "PyDict.hxx"

#endif /* PYDICT_HH_ */
