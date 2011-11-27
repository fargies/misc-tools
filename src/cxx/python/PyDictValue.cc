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

#include "PyDict.hh"
#include <typeinfo>

PyDictValue::PyDictValue(PyObject *dict, PyObject *key, PyObject *value) :
  PyValue(value), m_dict(dict), m_key(key)
{
  if (value == NULL) {
    value = PyDict_GetItem(dict, key);
    if (value)
      PyValue::operator = (value);
    else
      PyDict_SetItem(dict, key, Py_None);
  }
  Py_INCREF(key);
}

PyDictValue &PyDictValue::reseat(const PyDictValue &value)
{
  return reseat(value.m_dict, value.m_key, value.m_value);
}

PyDictValue &PyDictValue::reseat(PyObject *dict, PyObject *key, PyObject *value)
{
  Py_XINCREF(key);
  Py_XDECREF(m_key);

  m_dict = dict;
  m_key = key;

  if (value == NULL && dict != NULL && key != NULL) {
    value = PyDict_GetItem(dict, key);
    if (value)
      PyValue::operator = (value);
    else
      PyDict_SetItem(dict, key, Py_None);
  }
  else
    PyValue::operator = (value);

  return *this;
}

PyDictValue::PyDictValue(const PyDictValue &value) :
  PyValue(value), m_dict(value.m_dict), m_key(value.m_key)
{
  Py_XINCREF(m_key);
}

PyDictValue &PyDictValue::operator = (const PyDictValue &value)
{
  if (this != &value) {
    operator = (value.object());

    if (!m_dict && !m_key) {
        Py_XINCREF(value.m_key);
        m_dict = value.m_dict;
        m_key = value.m_key;
    }
  }
  return *this;
}

PyDictValue::PyDictValue() :
  PyValue(), m_dict(NULL), m_key(NULL)
{
}

PyDictValue::~PyDictValue()
{
  Py_XDECREF(m_key);
}

