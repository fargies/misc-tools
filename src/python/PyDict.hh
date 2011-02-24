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

#include <Python.h>
#include <map>
#include <string>
#include <iterator>

#include "PyError.hh"
#include "PyValue.hh"

/**
 * @brief a PyDict value
 * @details modifying this value directly modifies the dict's value
 */
class PyDictValue : public PyValue
{
public:
  /**
   * @details when value is NULL,  PyDict_GetItem is used to find out the real value
   * @details creates a Py_None value in the dict if the given key is not found
   */
  PyDictValue(PyObject *dict, PyObject *key, PyObject *value = NULL);
  PyDictValue(const PyDictValue &);
  PyDictValue &operator = (const PyDictValue &);
  PyDictValue();
  virtual ~PyDictValue();

  PyDictValue &operator = (const char *);
  PyDictValue &operator = (const std::string &);
  PyDictValue &operator = (int);
  PyDictValue &operator = (unsigned int);
  PyDictValue &operator = (PyObject *);
  PyDictValue &operator = (const PyValue &);

protected:
  mutable PyObject *m_dict, *m_key;
};

class PyDictIterator :
  public std::iterator<std::input_iterator_tag, std::pair<PyValue, PyDictValue > >
{
public:
  /**
   * @name typedefs
   * @{
   */
  typedef std::pair<PyValue, PyDictValue> value_type;
  typedef value_type & reference;
  typedef value_type * pointer;
  /**
   * @}
   */

  /**
   * @details iterate from the beginning of the dict
   */
  PyDictIterator(PyObject *obj = NULL);
  PyDictIterator(const PyDictIterator &);
  PyDictIterator &operator = (const PyDictIterator &);
  ~PyDictIterator();

  /**
   * @name operations
   * @{
   */
  PyDictIterator& operator ++();
  PyDictIterator operator ++(int);
  bool operator ==(const PyDictIterator &rhs) const;
  bool operator !=(const PyDictIterator &rhs) const;
  /**
   * @}
   */

  /**
   * @name element access
   * @{
   */
  reference operator *();
  pointer operator ->();
  /**
   * @}
   */

protected:
  /**
   * @details
   *  - Set to NULL if we're at the end of the iterator
   */
  PyObject *m_dict;

  /**
   * @details
   *  - Set to -1 when we're at the end of the iterator
   *  - 0 when we haven't started to iterate (used by PyDict::find)
   */
  Py_ssize_t m_pos;

  value_type m_value;

  void getNext();
  void clear();

  /**
   * @details protected constructor used to iterate directly on an item
   * (mostly used in PyDict::find)
   */
  PyDictIterator(PyObject *obj, PyObject *key, PyObject *value);

  /**
   * @details PyDict::find uses protected constructor
   */
  friend class PyDict;
};

class PyDict : public PyValue {
public:
  /**
   * @name typedefs
   * @{
   */
  typedef PyValue key_type;
  typedef PyDictValue mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef PyDictIterator iterator;
  typedef PyDictIterator const_iterator;
  typedef int size_type;
  /**
   * @}
   */

  PyDict();
  PyDict(PyObject *) throw (std::invalid_argument);
  PyDict(const PyDict &);
  PyDict &operator =(const PyDict &);

  ~PyDict();

  /**
   * @name capacity related methods
   * @{
   */
  bool empty() const;
  size_type size() const;
  size_type max_size() const; //TODO
  /**
   * @}
   */

  /**
   * @name iterators
   * @{
   */
  iterator begin();
  iterator end();
  /**
   * @}
   */

  /**
   * @name element access
   * @{
   */
  mapped_type operator[] (const key_type &);
  mapped_type operator[] (const char *);
  /**
   * @}
   */

  /**
   * @name modifiers
   * @{
   */
  std::pair<iterator, bool> insert(const value_type &); //TODO test
  iterator insert(iterator position, const value_type &); //TODO test
  template <class InputIterator>
        void insert(InputIterator first, InputIterator last);

  void erase(iterator &position); //TODO test
  size_type erase(const key_type &); //TODO test
  void erase(iterator first, iterator last); //TODO test

  // void swap(PyDict<K, V> &); FIXME should be implemented ?
  void clear(); //TODO test
  /**
   * @}
   */

  /**
   * @name operations
   * @note @ref iterator returned by @ref find should not be considered as a positional iterator,\n
   * using operator ++ on it will reset it's value and start an iteration from the beginning of the PyDict.
   * @{
   */
  iterator find(const key_type &);
  const_iterator find(const key_type &) const; //TODO
  size_type count(const key_type &) const;
  /**
   * @}
   */

  /**
   * @name templates
   * @note Convenience template.\n
   * Might be used with any PyValue convertible type.
   * @{
   */
  template <class K>
  inline size_type erase(const K &key)
  {
    return erase(PyValue(key));
  }

  template <class K>
  inline iterator find(const K &key)
  {
    return find(PyValue(key));
  }

  template <class K>
  inline size_type count(const K &key) const
  {
    return count(PyValue(key));
  }

  template <class K>
  inline mapped_type operator[] (const K &key)
  {
    return operator[](PyValue(key));
  }
  /**
   * @}
   */
};

#include "PyDict.hxx"

#endif /* PYDICT_HH_ */
