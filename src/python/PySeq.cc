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
** PySeq.cc
**
**        Created on: Mar 10, 2011
**   Orignial Author: fargie_s
**
*/

#include "PySeq.hh"

PySeq::PySeq(PyObject *obj) throw (std::invalid_argument)
{
  if (!PySequence_Check(obj))
    throw std::invalid_argument("Not a PySequence");
  PyValue::operator = (obj);
}

PySeq &PySeq::operator =(PyObject *obj) throw (std::invalid_argument)
{
  if (this->object() != obj) {
    if (!PySequence_Check(obj))
      throw std::invalid_argument("Not a PySequence");
    PyValue::operator = (obj);
  }
  return *this;
}

PySeq::~PySeq()
{
}

bool PySeq::operator ==(const PySeq &seq) const
{
  return PyValue::operator == (static_cast<const PyValue &> (seq));
}

bool PySeq::operator !=(const PySeq &seq) const
{
  return PyValue::operator != (static_cast<const PyValue &> (seq));
}

bool PySeq::empty() const
{
  return PySequence_Size(m_value) == 0;
}

PySeq::size_type PySeq::size() const
{
  return PySequence_Size(m_value);
}

void PySeq::resize(size_type n, const value_type &val)
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

PySeq::iterator PySeq::begin()
{
  return PySeq::iterator(m_value);
}

PySeq::iterator PySeq::end()
{
  return PySeq::iterator();
}

PySeq::reference PySeq::front()
{
  return PySeqValue(m_value, 0);
}

PySeq::reference PySeq::back()
{
  return PySeqValue(m_value, size() - 1);
}

/*
void PySeq::assign(PySeq::size_type n, const PySeq::value_type &u)
{
  m_value = PyType_GenericNew(m_value->ob_type, Py_None, Py_None); // Check that refcount is handled correctly
  Py_DECREF(m_value);
  PySequence_
  PySequence_InPlaceRepeat(m_value, u);
}
*/

/*
void PySeq::push_front(const PySeq::value_type &)
{
}*/

void PySeq::pop_front()
{
  PySequence_DelItem(m_value, 0);
}

/*void PySeq::push_back(const PySeq::value_type &)
{
}*/

void PySeq::pop_back()
{
  PySequence_DelItem(m_value, size() - 1);
}

/*PySeq::iterator PySeq::insert(PySeq::iterator position,
                              const PySeq::value_type &val)
{
}*/
/*
void PySeq::insert(PySeq::iterator position,
                   PySeq::size_type n,
                   const PySeq::value_type &val)
{
}
*/
void PySeq::erase(PySeq::iterator position)
{
  std::cout << "erase " << position.pos() << std::endl;
  PySequence_DelItem(m_value, position.pos());
}

void PySeq::erase(PySeq::iterator first, PySeq::iterator last)
{
  if (last.pos() == -1)
    PySequence_DelSlice(m_value, first.pos(), size());
  else
    PySequence_DelSlice(m_value, first.pos(), last.pos());
}

void PySeq::clear()
{
  PySequence_DelSlice(m_value, 0, size());
}

