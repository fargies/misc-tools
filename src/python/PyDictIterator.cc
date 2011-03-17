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
  m_dict(obj)
{
  if (m_dict) {
    m_pos = 0;
    if (!PyDict_Check(obj))
      throw PyError("Not a PyDict");
    Py_INCREF(m_dict);
    getNext();
  }
  else {
    m_pos = -1;
  }
}

PyDictIterator::PyDictIterator(PyObject *obj, PyObject *key, PyObject *value) :
    m_dict(obj), m_pos(0)
{
  if (!PyDict_Check(obj))
    throw PyError("Not a PyDict");

  m_value.first = key;
  m_value.second.reseat(m_dict, key, value);
  Py_INCREF(m_dict);
}

PyDictIterator::PyDictIterator(const PyDictIterator &it) :
  m_dict(it.m_dict), m_pos(it.m_pos), m_value(it.m_value)
{
  Py_XINCREF(m_dict);
}

PyDictIterator &PyDictIterator::operator = (const PyDictIterator &it)
{
  if (this != &it) {
    m_pos = it.m_pos;
    Py_XINCREF(it.m_dict);
    Py_XDECREF(m_dict);
    m_dict = it.m_dict;
    m_value.first = it.m_value.first;
    m_value.second.reseat(it.m_value.second);
  }
  return *this;
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

bool PyDictIterator::operator ==(const PyDictIterator &rhs) const
{
  return (m_dict == rhs.m_dict) && (m_pos == rhs.m_pos);
}

bool PyDictIterator::operator !=(const PyDictIterator &rhs) const
{
  return (m_dict != rhs.m_dict) || (m_pos != rhs.m_pos);
}

PyDictIterator::reference PyDictIterator::operator *() const
{
  return const_cast<PyDictIterator::reference>(m_value);
}

PyDictIterator::pointer PyDictIterator::operator ->() const
{
  return &const_cast<PyDictIterator::reference>(m_value);
}

void PyDictIterator::getNext()
{
  PyObject *key, *value;

  if (!PyDict_Next(m_dict, &m_pos, &key, &value)) {
    clear();
  }
  else {
    m_value.first = key;
    m_value.second.reseat(m_dict, key, value);
  }
}

void PyDictIterator::clear()
{
  if (m_dict) {
    Py_DECREF(m_dict);
    m_dict = NULL;
  }
  m_pos = -1;
  m_value.first = Py_None;
  m_value.second.reseat(NULL, NULL, NULL);
}

