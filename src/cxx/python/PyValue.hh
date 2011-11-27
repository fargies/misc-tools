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
** PyValue.hh
**
**        Created on: Feb 12, 2011
**            Author: fargie_s
**
*/

#ifndef PYVALUE_HH_
#define PYVALUE_HH_

#include <Python.h>
#include <string>
#include <iostream>

#include "PyError.hh"

/**
 * @class PyValue
 * @brief Convenience class to access PyObject
 */
class PyValue
{
public:
  /**
   * @brief create a Py_None object
   */
  PyValue();

  /**
   * @brief create a PyValue from a PyObject
   */
  explicit PyValue(PyObject *);
  explicit PyValue(const char *);
  explicit PyValue(const std::string &);
  explicit PyValue(int);
  explicit PyValue(unsigned int);

  PyValue(const PyValue &);
  PyValue &operator = (const PyValue &);

  virtual ~PyValue();

  PyValue &operator = (const char *);
  PyValue &operator = (const std::string &);
  PyValue &operator = (int);
  PyValue &operator = (unsigned int);
  PyValue &operator = (PyObject *);

  operator std::string() const;
  operator int() const;
  operator unsigned int() const;

  bool operator == (const char *str) const;
  bool operator == (const std::string &str) const;
  bool operator == (const PyValue &) const;
  bool operator == (PyObject *) const;

  template <typename T>
  bool operator == (const T &t) const {
    try {
      return ((T) *this) == t;
    }
    catch (PyError &e) {
      return false;
    }
  }

  template <typename T>
  bool operator != (const T &t) const {
    return !this->operator == (t);
  }

  /**
   * @brief return PyObject's type
   */
  inline const char *type() const
  {
    return m_value->ob_type->tp_name;
  }

  /**
   * @brief return internal PyObject
   */
  inline PyObject *object() const
  {
    return m_value;
  }

  /**
   * @brief get internal String buffer (for PyString objects)
   * @throw PyError when the object is not a PyString
   */
  const char *c_str() const throw (PyError);

protected:
  PyObject *m_value;
};

/**
 * @details Comparison template used to revert parameters
 * @param t the templated item to compare
 * @param val a PyValue item
 * @return true when items are equal
 */
template <typename T>
bool operator == (T t, const PyValue &val)
{
  return val.operator == (t);
}

/**
 * @details Simple difference operator
 * @param t the templated item to compare
 * @param val a PyValue item
 * @return true when items are not equal
 */
template <typename T>
bool operator != (T t, const PyValue &val)
{
  return !val.operator == (t);
}

/**
 * @brief ostream serialization
 * @details uses operator std::string to serialize data
 */
std::ostream &operator << (std::ostream &, const PyValue &);

#endif /* PYVALUE_HH_ */

