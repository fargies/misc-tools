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
** PyList.cc
**
**        Created on: Mar 17, 2011
**   Orignial Author: fargie_s
**
*/

#include <Python.h>

#include "PyList.hh"

PyList::PyList(PyObject *obj) throw (std::invalid_argument) :
  PySeq(obj)
{
  if (!PyList_Check(obj))
    throw std::invalid_argument("Not a PyList");
}

PyList &PyList::operator = (PyObject *obj) throw (std::invalid_argument)
{
  if (this->object() != obj) {
    if (!PyList_Check(obj))
      throw std::invalid_argument("Not a PyList");
    PySeq::operator = (obj);
  }
  return *this;
}

PyList::PyList(const PyList &list) :
  PySeq(PyList_New(0))
{
  PySequence_InPlaceConcat(m_value, list.m_value);
  Py_DECREF(m_value);
}

PyList &PyList::operator = (const PyList &list)
{
  if (this != &list) {
    PyValue::operator = (PyList_New(0));
    PySequence_InPlaceConcat(m_value, list.m_value);
    Py_XDECREF(m_value);
  }
  return *this;
}

PyList::PyList() :
  PySeq(PyList_New(0))
{
  Py_DECREF(m_value);
}

PyList::~PyList()
{
}

void PyList::assign(size_type n, const value_type &u)
{
  PyObject *obj = PyList_New(n);

  PyValue::operator = (obj);
  for (Py_ssize_t i = 0; i < n; ++i)
    PyList_SET_ITEM(obj, i, u.object());
  Py_DECREF(obj);
}

void PyList::push_front(const value_type &val)
{
  PyList_Insert(m_value, 0, val.object());
}

void PyList::push_back(const value_type &val)
{
  PyList_Append(m_value, val.object());
}

PyList::iterator PyList::insert(iterator position, const value_type &val)
{
  if (position.pos() == -1) {
    PyList_Append(m_value, val.object());
    return PyList::iterator(m_value, size() - 1, val.object());
  }
  else {
    PyList_Insert(m_value, position.pos(), val.object());
    return PyList::iterator(m_value, position.pos(), val.object());
  }
}

void PyList::insert(iterator position, size_type n, const value_type &c)
{
  Py_ssize_t pos = position.pos();

  if (pos == -1) {
    while (n--)
      PyList_Append(m_value, c.object());
  }
  else {
    while (n--)
      PyList_Insert(m_value, pos, c.object());
  }
}

