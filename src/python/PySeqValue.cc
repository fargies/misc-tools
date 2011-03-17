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
** PySeqValue.cc
**
**        Created on: Mar 17, 2011
**   Orignial Author: fargie_s
**
*/

#include "PySeq.hh"

PySeqValue::PySeqValue(PyObject *seq, Py_ssize_t index, PyObject *value) :
  PyValue(value), m_seq(seq), m_pos(index)
{
  if (value == NULL) {
    value = PySequence_ITEM(seq, index);
    if (PyErr_Occurred())
      PyErr_Clear();

    PyValue::operator = (value);
    Py_XDECREF(value);
  }
  else
    PyValue::operator = (value);
  Py_INCREF(seq);
}

PySeqValue &PySeqValue::reseat(const PySeqValue &value)
{
  return reseat(value.m_seq, value.m_pos, value.m_value);
}

PySeqValue &PySeqValue::reseat(PyObject *seq, Py_ssize_t index, PyObject *value)
{
  Py_XINCREF(seq);
  Py_XDECREF(m_seq);
  m_pos = index;
  m_seq = seq;

  if (value == NULL && seq != NULL) {
    value = PySequence_ITEM(seq, index);
    if (PyErr_Occurred())
      PyErr_Clear();

    PyValue::operator = (value);
    Py_XDECREF(value);
  }
  else
    PyValue::operator = (value);

  return *this;
}

PySeqValue::PySeqValue(const PySeqValue &value) :
  PyValue(value), m_seq(value.m_seq)
{
  Py_XINCREF(m_seq);
}

PySeqValue &PySeqValue::operator = (const PySeqValue &value)
{
  if (this != &value) {
    operator = (value.object());

    if (m_seq == NULL) {
      Py_XINCREF(value.m_seq);
      m_seq = value.m_seq;
      m_pos = value.m_pos;
    }
  }
  return *this;
}

PySeqValue::PySeqValue() :
  PyValue(), m_seq(NULL)
{
}

PySeqValue::~PySeqValue()
{
  Py_XDECREF(m_seq);
}

