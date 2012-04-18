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
** iterator.hh
**
**        Created on: Fri 04 Feb 2011 01:22:23 AM CET
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __ITERATOR_TEST__
#define __ITERATOR_TEST__

#include <list>
#include <map>
#include <vector>
#include <set>

template < typename V >
class Iterator
{
public:
  virtual void step() = 0;
  virtual V value() = 0;
  virtual bool empty() = 0;

  inline virtual Iterator &operator ++ ()
    { step(); return *this; };

  inline V operator * ()
    { return this->value(); };

  inline operator bool ()
    { return !this->empty(); };
};

template < class C, typename V >
class STLIterator : public Iterator< V >
{
public:
  STLIterator(C &l) :
    m_list(l), m_iter(l.begin())
  {}

  virtual void step() {
    ++m_iter;
  }
  virtual V value() {
    return *m_iter;
  }
  virtual bool empty() {
    return m_iter == m_list.end();
  }

protected:
  C &m_list;
  typename C::iterator m_iter;
};

template < typename V, typename K = void >
class STLIter {
public:
  typedef STLIterator< std::list< V >, V & > List;
  typedef STLIterator< std::vector< V >, V & > Vector;
  typedef STLIterator< std::set< V >, V & > Set;
  typedef STLIterator< std::map< V, K >, std::pair< V, K > > Map;
};

#endif

