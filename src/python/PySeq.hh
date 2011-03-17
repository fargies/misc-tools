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
** PySeq.hh
**
**        Created on: Mar 03, 2011
**   Orignial Author: fargie_s
**
*/

#ifndef PYSEQ_HH_
#define PYSEQ_HH_

#include <Python.h>
#include <list>
#include <string>
#include <iterator>

#include "PyError.hh"
#include "PyValue.hh"

class PySeqValue :
  public PyValue
{
public:
  PySeqValue(const PySeqValue &);
  PySeqValue &operator = (const PySeqValue &);
  PySeqValue();
  virtual ~PySeqValue();

  /**
   * @name templates
   * @note Convenience template.\n
   * Might be used with any PyValue convertible type.
   * @{
   */
  template <typename T>
  PySeqValue &operator = (const T &value)
  {
    if (m_seq)
      PySequence_SetItem(m_seq, m_pos,
          PyValue::operator = (value).object());
    else
      PyValue::operator = (value);
    return *this;
  }
  /**
   * @}
   */
protected:
  mutable PyObject *m_seq;
  Py_ssize_t m_pos;

  /**
   * @details when value is NULL, PySequence_ITEM is used to find out the real value
   */
  PySeqValue(PyObject *seq, Py_ssize_t index, PyObject *value = NULL);
  PySeqValue &reseat(PyObject *seq, Py_ssize_t index, PyObject *value = NULL);
  PySeqValue &reseat(const PySeqValue &value);

  friend class PySeq;
  friend class PySeqIterator;
};

class PySeqIterator :
  public std::iterator<std::input_iterator_tag, PySeqValue>
{
public:
  /**
   * @name typedefs
   * @{
   */
  typedef PySeqValue value_type;
  typedef value_type & reference;
  typedef value_type * pointer;
  /**
   * @}
   */

  /**
   * @details iterate from the beginning of the dict
   */
  PySeqIterator(PyObject *obj = NULL);
  PySeqIterator(const PySeqIterator &);
  PySeqIterator &operator = (const PySeqIterator &);
  ~PySeqIterator();

  /**
   * @name operations
   * @{
   */
  PySeqIterator &operator ++ ();
  PySeqIterator operator ++ (int);
  PySeqIterator &operator -- ();
  PySeqIterator operator -- (int);
  bool operator == (const PySeqIterator &rhs) const;
  bool operator != (const PySeqIterator &rhs) const;
  /**
   * @}
   */

  /**
   * @name element access
   * @{
   */
  reference operator *() const;
  pointer operator ->() const;
  /**
   * @}
   */

  /**
   * @return
   *  - current iterator position
   *  - -1 is end position
   */
  inline Py_ssize_t pos() const
  {
    return m_pos;
  }

protected:
  /**
   * @details
   *  - Set to NULL if we're at the end of the iterator
   */
  PyObject *m_seq;

  /**
   * @details
   *  - Set to -1 when we're at the end of the iterator
   *  - 0 when we haven't started to iterate (used by PySeq::find)
   */
  Py_ssize_t m_pos;

  value_type m_value;

  void getNext();
  void getPrev();
  void clear();

  /**
   * @details protected constructor used to iterate directly on an item
   */
  PySeqIterator(PyObject *obj, Py_ssize_t index, PyObject *value);

  /**
   * @details PyList::insert uses protected constructor
   */
  friend class PyList;
};

/**
 * @class PySeq
 * @brief Convenience class to access Python Lists
 */
class PySeq : public PyValue {
public:
  /**
   * @name typedefs
   * @{
   */
  typedef PyValue value_type;
  typedef PySeqValue reference;
  typedef const PySeqValue const_reference;
  typedef PySeqIterator iterator;
  typedef PySeqIterator const_iterator;
  typedef size_t size_type;
  /**
   * @}
   */

  /**
   * @brief create a list from a PyObject
   * @details does not duplicates PyObject's contents
   */
  PySeq(PyObject *) throw (std::invalid_argument);
  PySeq &operator =(PyObject *) throw (std::invalid_argument);

  ~PySeq();

  /**
   * @name comparison
   * @{
   */
  bool operator ==(const PySeq &) const;
  bool operator !=(const PySeq &) const;
  /**
   * @}
   */

  /**
   * @name capacity related methods
   * @{
   */
  bool empty() const;
  size_type size() const;
  inline size_type max_size() const throw ()
  {
    return size_t(-1) / sizeof(PyObject);
  };
  void resize(size_type n, const value_type &u = PyValue());
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
  reference front();
  reference back();
  /**
   * @}
   */

  /**
   * @name modifiers
   * @{
   */
  void pop_front();
  void pop_back();
  void erase(iterator position);
  void erase(iterator first, iterator last);
  void clear();
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
  inline void resize(size_type n, const K &val)
  {
    resize(n, PyValue(val));
  }

  /**
   * @}
   */
protected:
  /**
   * @brief copy existing PySeq
   * @details duplicates the PyObject
   * @warning set protected, deepcopy might be used
   */
  PySeq(const PySeq &);
  PySeq &operator =(const PySeq &);
};

#endif /* PYSEQ_HH_ */

