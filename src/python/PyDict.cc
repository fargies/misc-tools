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
** PyDict.cc
**
**        Created on: Feb 10, 2011
**            Author: fargie_s
**
*/

#include "PyDict.hh"

PyDict::PyDict(const PyDict &dict) :
  PyValue(PyDict_Copy(dict.object()))
{
  /* Drop the PyDict_Copy refcount (PyValue holds one now) */
  Py_DECREF(this->object());
}

PyDict::PyDict() :
  PyValue(PyDict_New())
{
  Py_DECREF(object());
}

PyDict &PyDict::operator =(const PyDict &dict)
{
  if (this != &dict) {
    PyValue::operator = (PyDict_Copy(dict.object()));
    Py_DECREF(this->object());
  }
  return *this;
}

PyDict::PyDict(PyObject *obj) throw (std::invalid_argument)
{
  if (!PyDict_Check(obj))
    throw std::invalid_argument("Not a PyDict");
  PyValue::operator = (obj);
}

PyDict &PyDict::operator =(PyObject *obj) throw (std::invalid_argument)
{
  if (this->object() != obj) {
    if (!PyDict_Check(obj))
      throw std::invalid_argument("Not a PyDict");
    PyValue::operator = (obj);
  }
  return *this;
}

PyDict::~PyDict()
{
}

bool PyDict::operator ==(const PyDict &dict) const
{
  return PyValue::operator == (static_cast<const PyValue &> (dict));
}

bool PyDict::operator !=(const PyDict &dict) const
{
  return PyValue::operator != (static_cast<const PyValue &> (dict));
}

bool PyDict::empty() const
{
  return PyDict_Size(m_value) == 0;
}

PyDict::size_type PyDict::size() const
{
  return PyDict_Size(m_value);
}

PyDict::iterator PyDict::begin()
{
  return PyDict::iterator(m_value);
}

PyDict::iterator PyDict::end()
{
  return PyDict::iterator();
}

PyDict::reference PyDict::operator[] (const PyDict::key_type &k)
{
  return PyDictValue(m_value, k.object());
}

PyDict::reference PyDict::operator[] (const char *k)
{
  return PyDictValue(m_value, PyValue(k).object(), PyDict_GetItemString(m_value, k));
}

std::pair<PyDict::iterator, bool> PyDict::insert(const PyDict::value_type &val)
{
  PyObject *obj = PyDict_GetItem(m_value, val.first.object());

  if (obj) {
    return make_pair(PyDict::iterator(m_value, val.first.object(), obj), false);
  }
  else {
    PyDict_SetItem(m_value, val.first.object(), val.second.object());
    return make_pair(PyDict::iterator(m_value, val.first.object(), val.second.object()), true);
  }
}

PyDict::iterator PyDict::insert(PyDict::iterator pos,
                                const PyDict::value_type &val)
{
  PyObject *obj = PyDict_GetItem(m_value, val.first.object());

  if (obj) {
    return PyDict::iterator(m_value, val.first.object(), obj);
  }
  else {
    PyDict_SetItem(m_value, val.first.object(), val.second.object());
    return PyDict::iterator(m_value, val.first.object(), val.second.object());
  }
}

template <class InputIterator>
void PyDict::insert(InputIterator first, InputIterator last)
{
  PyDict::iterator it = this->begin();
  while (first != last) {
    it = insert(it, *first);
    ++first;
  }
}

void PyDict::erase(const PyDict::iterator &pos)
{
  PyDict_DelItem(m_value, pos->first.object());
}

PyDict::size_type PyDict::erase(const PyDict::key_type &key)
{
  return (PyDict_DelItem(m_value, key.object()) == 0) ? 1 : 0;
}

void PyDict::erase(iterator first, iterator last)
{
  while (first != last) {
    erase(first);
    ++first;
  }
}

void PyDict::clear() {
  PyDict_Clear(m_value);
}

PyDict::iterator PyDict::find(const PyDict::key_type &key)
{
  PyObject *obj = PyDict_GetItem(m_value, key.object());

  if (obj)
    return PyDict::iterator(m_value, key.object(), obj);
  else
    return end();
}


PyDict::size_type PyDict::count(const PyDict::key_type &key) const
{
  return (PyDict_Contains(m_value, key.object()) == 1)? 1 : 0;
}
