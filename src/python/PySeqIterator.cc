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
** PySeqIterator.cc
**
**        Created on: Mar 17, 2011
**   Orignial Author: fargie_s
**
*/

#include "PySeq.hh"

PySeqIterator::PySeqIterator(PyObject *obj) :
  m_seq(obj)
{
  if (m_seq) {
    m_pos = -1;
    if (!PySequence_Check(obj))
      throw PyError("Not a PySequence");
    Py_INCREF(m_seq);
    getNext();
  }
  else {
    m_pos = -1;
  }
}

PySeqIterator::PySeqIterator(PyObject *obj, Py_ssize_t index, PyObject *value) :
  m_seq(obj), m_pos(index), m_value(obj, index, value)
{
  Py_INCREF(m_seq);
}

PySeqIterator::PySeqIterator(const PySeqIterator &it) :
  m_seq(it.m_seq), m_pos(it.m_pos), m_value(it.m_value)
{
  Py_XINCREF(m_seq);
}

PySeqIterator &PySeqIterator::operator = (const PySeqIterator &it)
{
  if (this != &it) {
    m_pos = it.m_pos;
    m_value.reseat(it.m_value);
    Py_XINCREF(it.m_seq);
    Py_XDECREF(m_seq);
    m_seq = it.m_seq;
  }
  return *this;
}

PySeqIterator::~PySeqIterator()
{
  Py_XDECREF(m_seq);
}

PySeqIterator &PySeqIterator::operator ++()
{
  getNext();
  return *this;
}

PySeqIterator PySeqIterator::operator ++(int)
{
  PySeqIterator it(*this);
  getNext();
  return it;
}

PySeqIterator &PySeqIterator::operator --()
{
  getPrev();
  return *this;
}

PySeqIterator PySeqIterator::operator --(int)
{
  PySeqIterator it(*this);
  getPrev();
  return it;
}

bool PySeqIterator::operator ==(const PySeqIterator &rhs) const
{
  return (m_seq == rhs.m_seq) && (m_pos == rhs.m_pos);
}

bool PySeqIterator::operator !=(const PySeqIterator &rhs) const
{
  return (m_seq != rhs.m_seq) || (m_pos != rhs.m_pos);
}

void PySeqIterator::getNext()
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

void PySeqIterator::getPrev()
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

void PySeqIterator::clear()
{
  m_pos = -1;
  m_value.reseat(NULL, NULL, NULL);
  Py_XDECREF(m_seq);
  m_seq = NULL;
}

PySeqIterator::reference PySeqIterator::operator *() const
{
  return const_cast<PySeqIterator::reference>(m_value);
}

PySeqIterator::pointer PySeqIterator::operator ->() const
{
  return &const_cast<PySeqIterator::reference>(m_value);
}

