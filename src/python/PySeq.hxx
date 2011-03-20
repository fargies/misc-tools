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
** PySeq.hxx
**
**        Created on: Mar 19, 2011
**   Orignial Author: fargie_s
**
*/

#ifndef __PYSEQ_HXX__
#define __PYSEQ_HXX__

#include "PySeq.hh"

template <typename T>
PySeqBase<T>::PySeqBase(PyObject *obj) throw (std::invalid_argument)
{
  if (!PySequence_Check(obj))
    throw std::invalid_argument("Not a PySequence");
  PyValue::operator = (obj);
}

template <typename T>
PySeqBase<T> &PySeqBase<T>::operator =(PyObject *obj) throw (std::invalid_argument)
{
  if (this->object() != obj) {
    if (!PySequence_Check(obj))
      throw std::invalid_argument("Not a PySequence");
    PyValue::operator = (obj);
  }
  return *this;
}

template <typename T>
PySeqBase<T>::~PySeqBase()
{
}

template <typename T>
bool PySeqBase<T>::operator ==(const PySeqBase<T> &seq) const
{
  return PyValue::operator == (static_cast<const PyValue &> (seq));
}

template <typename T>
bool PySeqBase<T>::operator !=(const PySeqBase<T> &seq) const
{
  return PyValue::operator != (static_cast<const PyValue &> (seq));
}

template <typename T>
bool PySeqBase<T>::empty() const
{
  return PySequence_Size(m_value) == 0;
}

template <typename T>
typename PySeqBase<T>::size_type PySeqBase<T>::size() const
{
  return PySequence_Size(m_value);
}

template <typename T>
void PySeqBase<T>::resize(size_type n, const value_type &val)
{
  size_type sz = size();

  if (sz > n) {
    PySequence_DelSlice(m_value, n, sz - 1);
  }
  else {
    PyObject *seq = PySequence_InPlaceRepeat(val.object(), n - sz);
    PySequence_InPlaceConcat(m_value, seq);
    Py_DECREF(seq);
  }
}

template <typename T>
typename PySeqBase<T>::iterator PySeqBase<T>::begin()
{
  return PySeqBase<T>::iterator(m_value);
}

template <typename T>
typename PySeqBase<T>::iterator PySeqBase<T>::end()
{
  return PySeqBase<T>::iterator();
}

template <typename T>
typename PySeqBase<T>::reference PySeqBase<T>::front()
{
  return PySeqValue<T>(m_value, 0);
}

template <typename T>
typename PySeqBase<T>::reference PySeqBase<T>::back()
{
  return PySeqValue<T>(m_value, size() - 1);
}

template <typename T>
void PySeqBase<T>::pop_front()
{
  PySequence_DelItem(m_value, 0);
}

template <typename T>
void PySeqBase<T>::pop_back()
{
  PySequence_DelItem(m_value, size() - 1);
}

template <typename T>
void PySeqBase<T>::erase(PySeqBase<T>::iterator position)
{
  std::cout << "erase " << position.pos() << std::endl;
  PySequence_DelItem(m_value, position.pos());
}

template <typename T>
void PySeqBase<T>::erase(PySeqBase<T>::iterator first,
                         PySeqBase<T>::iterator last)
{
  if (last.pos() == -1)
    PySequence_DelSlice(m_value, first.pos(), size());
  else
    PySequence_DelSlice(m_value, first.pos(), last.pos());
}

template <typename T>
void PySeqBase<T>::clear()
{
  PySequence_DelSlice(m_value, 0, size());
}

#endif

