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
** PyList.hxx
**
**        Created on: Mar 20, 2011
**   Orignial Author: fargie_s
**
*/

#ifndef __PYLIST_HXX__
#define __PYLIST_HXX__

#include "PyList.hh"

template <typename V>
PyListBase<V>::PyListBase(PyObject *obj) throw (std::invalid_argument) :
  PySeqBase<V>(obj)
{
  if (!PyList_Check(obj))
    throw std::invalid_argument("Not a PyList");
}

template <typename V>
PyListBase<V> &PyListBase<V>::operator = (PyObject *obj) throw (std::invalid_argument)
{
  if (this->object() != obj) {
    if (!PyList_Check(obj))
      throw std::invalid_argument("Not a PyList");
    PySeqBase<V>::operator = (obj);
  }
  return *this;
}

template <typename V>
PyListBase<V>::PyListBase(const PyListBase &list) :
  PySeqBase<V>(PyList_New(0))
{
  PySequence_InPlaceConcat(this->m_value, list.m_value);
  Py_DECREF(this->m_value);
}

template <typename V>
PyListBase<V> &PyListBase<V>::operator = (const PyListBase &list)
{
  if (this != &list) {
    PyValue::operator = (PyList_New(0));
    PySequence_InPlaceConcat(this->m_value, list.m_value);
    Py_XDECREF(this->m_value);
  }
  return *this;
}

template <typename V>
PyListBase<V>::PyListBase() :
  PySeqBase<V>(PyList_New(0))
{
  Py_DECREF(this->m_value);
}

template <typename V>
PyListBase<V>::~PyListBase()
{
}

template <typename V>
template <typename K>
bool PyListBase<V>::operator ==(const PyListBase<K> &list) const
{
  return PySeqBase<V>::operator == (list);
}

template <typename V>
template <typename K>
bool PyListBase<V>::operator !=(const PyListBase<K> &list) const
{
  return PySeqBase<V>::operator != (list);
}

template <typename V>
void PyListBase<V>::assign(size_type n, const value_type &u)
{
  PyObject *obj = PyList_New(n);

  PyValue::operator = (obj);
  for (Py_ssize_t i = 0; i < n; ++i)
    PyList_SET_ITEM(obj, i, u.object());
  Py_DECREF(obj);
}

template <typename V>
void PyListBase<V>::push_front(const value_type &val)
{
  PyList_Insert(this->m_value, 0, val.object());
}

template <typename V>
void PyListBase<V>::push_back(const value_type &val)
{
  PyList_Append(this->m_value, val.object());
}

template <typename V>
typename PyListBase<V>::iterator PyListBase<V>::insert(iterator position, const value_type &val)
{
  if (position.pos() == -1) {
    PyList_Append(this->m_value, val.object());
    return typename PyListBase<V>::iterator(this->m_value, this->size() - 1, val.object());
  }
  else {
    PyList_Insert(this->m_value, position.pos(), val.object());
    return typename PyListBase<V>::iterator(this->m_value, position.pos(), val.object());
  }
}

template <typename V>
void PyListBase<V>::insert(iterator position, size_type n, const value_type &c)
{
  Py_ssize_t pos = position.pos();

  if (pos == -1) {
    while (n--)
      PyList_Append(this->m_value, c.object());
  }
  else {
    while (n--)
      PyList_Insert(this->m_value, pos, c.object());
  }
}

#endif

