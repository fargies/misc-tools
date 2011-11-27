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
** PySeqValue.hxx
**
**        Created on: Mar 20, 2011
**   Orignial Author: fargie_s
**
*/

#ifndef __PYSEQVALUE_HXX__
#define __PYSEQVALUE_HXX__

#include "PySeq.hh"

template <typename V>
PySeqValue<V>::PySeqValue(PyObject *seq, Py_ssize_t index, PyObject *value) :
  V(value), m_seq(seq), m_pos(index)
{
  if (value == NULL) {
    value = PySequence_ITEM(seq, index);
    if (PyErr_Occurred())
      PyErr_Clear();

    V::operator = (value);
    Py_XDECREF(value);
  }
  else
    V::operator = (value);
}

template <typename V>
PySeqValue<V> &PySeqValue<V>::reseat(const PySeqValue<V> &value)
{
  return reseat(value.m_seq, value.m_pos, value.m_value);
}

template <typename V>
PySeqValue<V> &PySeqValue<V>::reseat(PyObject *seq, Py_ssize_t index, PyObject *value)
{
  m_pos = index;
  m_seq = seq;

  if (value == NULL && seq != NULL) {
    value = PySequence_ITEM(seq, index);
    if (PyErr_Occurred())
      PyErr_Clear();

    V::operator = (value);
    Py_XDECREF(value);
  }
  else
    V::operator = (value);

  return *this;
}

template <typename V>
PySeqValue<V>::PySeqValue(const PySeqValue<V> &value) :
  V(value), m_seq(value.m_seq)
{
}

template <typename V>
PySeqValue<V> &PySeqValue<V>::operator = (const PySeqValue<V> &value)
{
  if (this != &value) {
    operator = (value.object());

    if (m_seq == NULL) {
      m_seq = value.m_seq;
      m_pos = value.m_pos;
    }
  }
  return *this;
}

template <typename V>
PySeqValue<V>::PySeqValue() :
  V(), m_seq(NULL)
{
}

template <typename V>
PySeqValue<V>::~PySeqValue()
{
}

#endif

