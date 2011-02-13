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
** PyDictValue.cc
**
**        Created on: Feb 12, 2011
**            Author: fargie_s
**
*/

#include <typeinfo>
#include "PyDict.hh"

using namespace std;

PyDictValue::PyDictValue(PyObject *dict, PyObject *key, PyObject *value) :
  m_dict(dict), m_key(key), m_value(value)
{
  Py_INCREF(dict);
  Py_INCREF(key);
  Py_XINCREF(value);
}

PyDictValue::PyDictValue() :
  m_dict(NULL), m_key(NULL), m_value(NULL)
{
}

PyDictValue::~PyDictValue()
{
  Py_XDECREF(m_value);
  Py_XDECREF(m_dict);
  Py_XDECREF(m_key);
}

void PyDictValue::set(PyObject *dict, PyObject *key, PyObject *value)
{
  Py_XDECREF(m_value);
  Py_XDECREF(m_dict);
  Py_XDECREF(m_key);

  m_dict = dict;
  m_key = key;
  m_value = value;
  Py_XINCREF(m_dict);
  Py_XINCREF(m_key);
  Py_XINCREF(m_value);
}

PyDictValue &PyDictValue::operator = (const char *value)
{
  setValue(PyString_FromString(value));
  return *this;
}

PyDictValue &PyDictValue::operator = (const std::string &value)
{
  return this->operator = (value.c_str());
}

PyDictValue &PyDictValue::operator = (int value)
{
 setValue(PyInt_FromLong(value));
 return *this;
}

PyDictValue &PyDictValue::operator = (unsigned int value)
{
 setValue(PyInt_FromLong(value));
 return *this;
}

PyDictValue::operator const char *() const
{
  PyObject *obj = getValue();
  if (!obj)
    throw PyError("Can't cast empty PyDictValue");
  const char *ret = PyString_AsString(obj);

  if (PyErr_Occurred()) {
    PyErr_Clear();
    throw PyError("Can't cast in string");
  }
  return ret;
}

PyDictValue::operator int() const
{
  PyObject *obj = getValue();
  if (!obj)
    throw PyError("Can't cast empty PyDictValue");
  long ret = PyInt_AsLong(obj);

  if (PyErr_Occurred()) {
    PyErr_Clear();
    throw PyError("Can't cast in int");
  }

  return (int) ret;
}

PyDictValue::operator unsigned int() const
{
  PyObject *obj = getValue();
  if (!obj)
    throw PyError("Can't cast empty PyDictValue");
  long ret = PyInt_AsLong(obj);

  if (PyErr_Occurred()) {
    PyErr_Clear();
    throw PyError("Can't cast in int");
  }

  return (unsigned int) ret;
}

bool PyDictValue::operator == (const char *str) const
{
  try {
    return ::strcmp(str, this->operator const char *()) == 0;
  }
  catch (PyError &e) {
    return false;
  }
}

bool PyDictValue::operator == (const std::string &str) const
{
  return this->operator == (str.c_str());
}
