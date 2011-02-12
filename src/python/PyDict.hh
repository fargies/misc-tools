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
** PyDict.hh
**
**        Created on: Feb 7, 2011
**            Author: fargie_s
**
*/

#ifndef PYDICT_HH_
#define PYDICT_HH_

#include <map>
#include <iterator>
#include <stdexcept>
#include <Python.h>

class PyError : public std::logic_error
{
public:
  PyError(const std::string &what) :
    std::logic_error(what)
  {}
};

class PyDictValue
{
public:
  PyDictValue(PyObject *dict, PyObject *key, PyObject *value = NULL);
  PyDictValue();
  ~PyDictValue();

  PyDictValue &operator = (const char *);
  PyDictValue &operator = (const std::string &);
  PyDictValue &operator = (int);
  PyDictValue &operator = (unsigned int);

  operator const char * () const;
  operator std::string() const;
  operator int() const;
  operator unsigned int() const;

  void set(PyObject *dict, PyObject *key, PyObject *value);
  inline void clear() {
    set(NULL, NULL, NULL);
  }

protected:
  mutable PyObject *m_dict, *m_key, *m_value;

  inline bool setValue(PyObject *obj) throw (PyError) {
    if (m_dict && m_key && obj) {
      PyDict_SetItem(m_dict, m_key, obj);

      if (m_value)
        Py_DECREF(m_value);
      m_value = obj;
    }
    else {
      Py_XDECREF(obj);
      throw PyError("Can't set PyDictValue on NULL dict");
    }
  }

  inline PyObject *getValue() const {
    if (!m_value && m_dict && m_key) {
      m_value = PyDict_GetItem(m_dict, m_key);
      Py_XINCREF(m_value);
    }
    return m_value;
  }
};

class PyDictIterator :
  public std::iterator<std::input_iterator_tag, std::pair<std::string, PyDictValue > >
{
public:
  typedef std::pair<std::string, PyDictValue> value_type;
  typedef value_type & reference;
  typedef value_type * pointer;

  /**
   * @details iterate from the beginning of the dict
   */
  PyDictIterator(PyObject *obj = NULL);
  PyDictIterator(const PyDictIterator &);
  ~PyDictIterator();
  PyDictIterator& operator ++();
  PyDictIterator operator ++(int);
  bool operator ==(const PyDictIterator &rhs);
  bool operator !=(const PyDictIterator &rhs);

  reference operator *();
  pointer operator ->();

protected:
  /**
   * @details
   *  - Set to NULL if we're at the end of the iterator
   */
  PyObject *m_dict;

  /**
   * @details
   *  - Set to -1 when the iterator can't be moved
   *    (when using find on the PyDict).
   *  - 0 when we're at the beginnig of the iterator
   */
  Py_ssize_t m_pos;

  value_type m_value;

  void getNext();
  void clear();
};

class PyDict {
public:
  /**
   * @brief key_type defaults to string
   */
  typedef std::string key_type;
  typedef PyDictValue mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef PyDictIterator iterator;
  typedef PyDictIterator const_iterator;
  typedef int size_type;
  //typedef size_t size_type;

  PyDict(PyObject *) throw (std::invalid_argument);
  PyDict(const PyDict &);
  ~PyDict();

  PyDict &operator =(const PyDict &);

  /* capacity */
  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  /* iterators */
  iterator begin();
  iterator end();

  /* element access */
  mapped_type &operator[] (const key_type &);

  /* modifiers */
  std::pair<iterator, bool> insert(const value_type &);
  iterator insert(iterator position, const value_type &);
  template <class InputIterator>
        void insert(InputIterator first, InputIterator last);
  void erase(iterator &position);
  size_type erase(const key_type &);
  void erase(iterator first, iterator last);
  // void swap(PyDict<K, V> &); FIXME should be implemented ?
  void clear();

  /* operations */
  iterator find(const key_type &);
  const_iterator find(const key_type &) const;
  size_type count(const key_type &) const;

protected:
  PyObject *m_dict;
};

#include "PyDict.hxx"

#endif /* PYDICT_HH_ */
