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
** PyValue.cc
**
**        Created on: Feb 12, 2011
**            Author: fargie_s
**
*/

#include "PyValue.hh""

PyValue::PyValue() :
  m_obj(Py_None)
{
  Py_INCREF(m_obj);
}

PyValue::PyValue(PyObject *obj) :
  m_obj(obj)
{
  Py_INCREF(m_obj);
}

PyValue::PyValue(const PyValue &val) :
  m_obj(val.m_obj)
{
  Py_INCREF(m_obj);
}

PyValue::~PyValue()
{
  Py_DECREF(m_obj);
}

PyValue &PyValue::operator = (const char *str)
{
  Py_XDECREF(m_obj);
  m_obj = PyString_FromString(str);
  return *this;
}

PyValue &PyValue::operator = (const std::string &str)
{
  Py_XDECREF(m_obj);
  m_obj = PyString_FromString(str.c_str());
  return *this;
}

PyValue &PyValue::operator = (int i)
{
  Py_XDECREF(m_obj);
  m_obj = PyInt_FromLong(i);
  return *this;
}

PyValue &PyValue::operator = (unsigned int i)
{
  Py_XDECREF(m_obj);
  m_obj = PyInt_FromLong(i);
  return *this;
}

PyValue &PyValue::operator = (PyObject *obj)
{
  Py_XINCREF(obj);
  Py_XDECREF(m_obj);
  m_obj = obj;
  return *this;
}

PyValue &PyValue::operator = (const PyValue &val)
{
  Py_INCREF(val.m_obj);
  Py_DECREF(m_obj);
  m_obj = val.m_obj;
  return *this;
}

PyValue::operator std::string () const
{
  return this->c_str();
}

PyValue::operator int() const
{
  long ret = PyInt_AsLong(m_obj);
  if (PyErr_Occurred()) {
    PyErr_Clear();
    throw PyError("Can't cast PyValue as int");
  }
  return (int) ret;
}

PyValue::operator unsigned int() const
{
  long ret = PyInt_AsLong(m_obj);
  if (PyErr_Occurred()) {
    PyErr_Clear();
    throw PyError("Can't cast PyValue as unsigned int");
  }
  return (unsigned int) ret;
}

bool PyValue::operator == (const char *str) const
{
  try {
    return ::strcmp(str, this->c_str()) == 0;
  }
  catch (PyError &e) {
    return false;
  }
}

bool PyValue::operator == (const std::string &str) const
{
  return this->operator == (str.c_str());
}

bool PyValue::operator == (const PyValue &val) const
{
  return PyObject_RichCompareBool(val.m_obj, this->m_obj, Py_EQ) == 1;
}

bool PyValue::operator == (PyObject *obj) const
{
  return PyObject_RichCompareBool(obj, this->m_obj, Py_EQ) == 1;
}

const char *PyValue::c_str() const throw (PyError)
{
  const char *ret = PyString_AsString(m_obj);

  if (PyErr_Occurred()) {
    PyErr_Clear();
    throw PyError("Can't get internal string buffer");
  }
  return ret;
}
