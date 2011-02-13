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
  m_dict(dict.m_dict)
{
  Py_INCREF(dict.m_dict);
}

PyDict::~PyDict()
{
  Py_DECREF(m_dict);
}

PyDict &PyDict::operator =(const PyDict &dict)
{
  if (this != &dict) {
    if (m_dict)
      Py_DECREF(m_dict);
    m_dict = dict.m_dict;
    Py_INCREF(m_dict);
  }
  return *this;
}

bool PyDict::empty() const
{
  return PyDict_Size(m_dict) == 0;
}

PyDict::size_type PyDict::size() const
{
  return PyDict_Size(m_dict);
}

PyDict::size_type PyDict::max_size() const
{
  return 0; //TODO
}

PyDict::iterator PyDict::begin()
{
  return PyDict::iterator(m_dict);
}

PyDict::iterator PyDict::end()
{
  return PyDict::iterator();
}

/**
 * TODO
 * create/use const iterators

template <typename K, typename V>
typename PyDict<K, V>::const_iterator PyDict<K, V>::begin() const
{
  return PyDict<K, V>::const_iterator(m_dict);
}

template <typename K, typename V>
typename PyDict<K, V>::const_iterator end() const
{
  return PyDict<K, V>::const_iterator(m_dict, this->size());
}
*/
/*
template <typename K, typename V>
typename PyDict<K, V>::mapped_type &operator[] (const PyDict<K, V>::key_type &k)
{
}
*/

std::pair<PyDict::iterator, bool> PyDict::insert(const PyDict::value_type &val)
{
}

PyDict::iterator PyDict::insert(PyDict::iterator pos,
                                const PyDict::value_type &val)
{
}

template <class InputIterator>
void PyDict::insert(InputIterator first, InputIterator last)
{
  PyDict::iterator it = this->begin();
  while (first != last) {
    it = insert(it, first);
    ++first;
  }
}

void PyDict::erase(PyDict::iterator &pos)
{
  PyDict_DelItemString(m_dict, pos->first.c_str());
}

PyDict::size_type PyDict::erase(const PyDict::key_type &key)
{
  return (PyDict_DelItemString(m_dict, key.c_str()) == 0) ? 1 : 0;
}

void PyDict::erase(iterator first, iterator last)
{
  while (first != last) {
    erase(first);
    ++first;
  }
}

void PyDict::clear() {
  PyDict_Clear(m_dict);
}

/*PyDict::iterator PyDict::find(const std::string &key)
{
}*/
