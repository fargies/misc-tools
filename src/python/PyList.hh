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
** PyList.hh
**
**        Created on: Mar 17, 2011
**   Orignial Author: fargie_s
**
*/

#ifndef PYLIST_HH_
#define PYLIST_HH_

#include "PySeq.hh"

class PyList :
  public PySeq
{
public:
  /**
   * @brief create a list from a PyObject
   * @details does not duplicates PyObject's contents
   */
  PyList(PyObject *) throw (std::invalid_argument);
  PyList &operator = (PyObject *) throw (std::invalid_argument);

  /**
   * @brief copy an existing PyList
   * @details duplicates the PyObject
   */
  PyList(const PyList &list);
  PyList &operator = (const PyList &list);

  /**
   * @brief create a new list
   */
  PyList();

  ~PyList();

  /**
   * @name modifiers
   * @{
   */
  void assign(size_type n, const value_type &u = PyValue());
  void push_front(const value_type &);
  void push_back(const value_type &);
  iterator insert(iterator position, const value_type &);
  void insert(iterator position, size_type n, const value_type &c);
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
  inline void assign(size_type n, const K &val)
  {
    assign(n, PyValue(val));
  }

  template <class K>
  inline void push_front(const K &val)
  {
    push_front(PyValue(val));
  }

  template <class K>
  inline void push_back(const K &val)
  {
    push_back(PyValue(val));
  }

  template <class K>
  inline iterator insert(iterator position, const K &val)
  {
    return insert(position, PyValue(val));
  }

  template <class K>
  inline void insert(iterator position, size_type n, const K &val)
  {
    insert(position, n, PyValue(val));
  }
/*  template <class InputIterator>
  inline void assign(InputIterator first, InputIterator last)
  {
  }//FIXME

  template <class InputIterator>
  inline void insert(iterator position, InputIterator first, InputIterator last)
  {
    PySeq::iterator it = this->begin();
    while (first != last) {
      position = insert(position, *first);
      ++first;
    }
  }*/
  /**
   * @}
   */


};

#endif

