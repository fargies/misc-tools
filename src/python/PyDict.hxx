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
** PyDict.hxx
**
**        Created on: Feb 7, 2011
**            Author: fargie_s
**
*/

#ifndef PYDICT_HXX_
#define PYDICT_HXX_

#include <stdexcept>
#include <string>
#include "PyDict.hh"


template <typename T>
PyDictIterator<T>::PyDictIterator(PyObject *obj) :
  m_dict(obj), m_pos(0)
{
  if (!PyDict_Check(obj))
    throw std::invalid_argument("Not a PyDict");
  Py_INCREF(m_dict);
}

template <typename T>
PyDictIterator<T>::PyDictIterator(const PyDictIterator< T > &it) :
  m_dict(obj), m_pos(it.m_pos)
{
  Py_INCREF(m_dict);
}

template <typename T>
PyDictIterator<T>::~PyDictIterator()
{
  Py_DECREF(m_dict);
}

template <typename T>
PyDictIterator &PyDictIterator<T>::operator ++() {
  m_pos++;
  return *this;
}

template <typename T>
PyDictIterator PyDictIterator<T>::operator ++(int) {
  PyDictIterator it(*this);
  m_pos++;
  return it;
}

template <typename T>
bool PyDictIterator<T>::operator ==(const PyDictIterator<T> &rhs)
{
  return (m_dict == rhs.m_dict) && (m_pos == rhs.m_pos);
}

template <typename T>
bool PyDictIterator<T>::operator !=(const PyDictIterator<T> &rhs)
{
  return (m_dict != rhs.m_dict) || (m_pos != rhs.m_pos);
}

template <>
std::pair< std::string, std::string > &PyDictIterator::operator *()
{
  PyObject *key, *value;
  if (PyDict_Next(m_dict, &m_pos, &key, &value)) {
    m_value.first = PyString_AsString(key);
    m_value.second = PyString_AsString(value);
  }
  else {
    // FIXME should throw something ?
    m_value.first.clear();
    m_value.second.clear();
  }
  return m_value;
}

#endif /* PYDICT_HXX_ */
