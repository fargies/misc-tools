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
** PyDictIterator.cc
**
**        Created on: Feb 12, 2011
**            Author: fargie_s
**
*/

#include "PyDict.hh"

#include <stdexcept>
#include <string>
#include "PyDict.hh"

PyDictIterator::PyDictIterator(PyObject *obj) :
  m_dict(obj), m_pos(0)
{
  if (m_dict) {
    m_pos = 0;
    if (!PyDict_Check(obj))
      throw std::invalid_argument("Not a PyDict");
    Py_INCREF(m_dict);
  }
  else {
    m_pos = -1;
  }
}

PyDictIterator::PyDictIterator(const PyDictIterator &it) :
  m_dict(it.m_dict), m_pos(it.m_pos)
{
  Py_INCREF(m_dict);
}

PyDictIterator::~PyDictIterator()
{
  Py_XDECREF(m_dict);
}

PyDictIterator &PyDictIterator::operator ++() {
  getNext();
  return *this;
}

PyDictIterator PyDictIterator::operator ++(int) {
  PyDictIterator it(*this);
  getNext();
  return it;
}

bool PyDictIterator::operator ==(const PyDictIterator &rhs)
{
  return (m_dict == rhs.m_dict) && (m_pos == rhs.m_pos);
}

bool PyDictIterator::operator !=(const PyDictIterator &rhs)
{
  return (m_dict != rhs.m_dict) || (m_pos != rhs.m_pos);
}

PyDictIterator::reference PyDictIterator::operator *()
{
  return m_value;
}

PyDictIterator::pointer PyDictIterator::operator ->()
{
  return &m_value;
}

void PyDictIterator::getNext()
{
  PyObject *key, *value;

  if (!PyDict_Next(m_dict, &m_pos, &key, &value)) {
    clear();
  }
  else {
    m_value.first = PyString_AsString(key);
    m_value.second.set(m_dict, key, value);
  }
}

void PyDictIterator::clear()
{
  //FIXME m_value still contains something
  m_pos = -1;
  m_value.first.clear();
  m_value.second.clear();
}

PyDict::PyDict(PyObject *obj) throw (std::invalid_argument):
  m_dict(obj)
{
  if (!PyDict_Check(obj))
    throw std::invalid_argument("Not a PyDict");
  Py_INCREF(m_dict);
}
