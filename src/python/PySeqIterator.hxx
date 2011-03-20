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
** PySeqIterator.hxx
**
**        Created on: Mar 20, 2011
**   Orignial Author: fargie_s
**
*/

#ifndef __PYSEQITERATOR_HXX__
#define __PYSEQITERATOR_HXX__

#include "PySeq.hh"

template <typename V>
PySeqIterator<V>::PySeqIterator(PyObject *seq) :
  m_seq(seq)
{
  if (m_seq != NULL) {
    m_pos = -1;
    if (!PySequence_Check(seq))
      throw PyError("Not a PySequence");
    getNext();
  }
  else {
    m_pos = -1;
  }
}

template <typename V>
PySeqIterator<V>::PySeqIterator(PyObject *obj, Py_ssize_t index, PyObject *value) :
  m_seq(obj), m_pos(index), m_value(obj, index, value)
{
}

template <typename V>
PySeqIterator<V>::PySeqIterator(const PySeqIterator<V> &it) :
  m_seq(it.m_seq), m_pos(it.m_pos), m_value(it.m_value)
{
}

template <typename V>
PySeqIterator<V> &PySeqIterator<V>::operator = (const PySeqIterator<V> &it)
{
  if (this != &it) {
    m_pos = it.m_pos;
    m_seq = it.m_seq;
    m_value.reseat(it.m_value);
  }
  return *this;
}

template <typename V>
PySeqIterator<V>::~PySeqIterator()
{
}

template <typename V>
PySeqIterator<V> &PySeqIterator<V>::operator ++()
{
  getNext();
  return *this;
}

template <typename V>
PySeqIterator<V> PySeqIterator<V>::operator ++(int)
{
  PySeqIterator it(*this);
  getNext();
  return it;
}

template <typename V>
PySeqIterator<V> &PySeqIterator<V>::operator --()
{
  getPrev();
  return *this;
}

template <typename V>
PySeqIterator<V> PySeqIterator<V>::operator --(int)
{
  PySeqIterator it(*this);
  getPrev();
  return it;
}

template <typename V>
bool PySeqIterator<V>::operator ==(const PySeqIterator &rhs) const
{
  return (m_seq == rhs.m_seq) && (m_pos == rhs.m_pos);
}

template <typename V>
bool PySeqIterator<V>::operator !=(const PySeqIterator &rhs) const
{
  return (m_seq != rhs.m_seq) || (m_pos != rhs.m_pos);
}

template <typename V>
void PySeqIterator<V>::getNext()
{
  PyObject *obj = PySequence_ITEM(m_seq, ++m_pos);

  if (PyErr_Occurred()) {
    PyErr_Clear();
    clear();
  }
  else {
    m_value.reseat(m_seq, m_pos, obj);
    Py_DECREF(obj);
  }
}

template <typename V>
void PySeqIterator<V>::getPrev()
{
  PyObject *obj = PySequence_ITEM(m_seq, --m_pos);

  if (PyErr_Occurred()) {
    PyErr_Clear();
    clear();
  } else {
    m_value.reseat(m_seq, m_pos, obj);
    Py_DECREF(obj);
  }
}

template <typename V>
void PySeqIterator<V>::clear()
{
  m_pos = -1;
  m_value.reseat(NULL, NULL, NULL);
  m_seq = NULL;
}

template <typename V>
typename PySeqIterator<V>::reference PySeqIterator<V>::operator *() const
{
  return const_cast<PySeqIterator<V>::reference>(m_value);
}

template <typename V>
typename PySeqIterator<V>::pointer PySeqIterator<V>::operator ->() const
{
  return &const_cast<PySeqIterator<V>::reference>(m_value);
}

#endif

