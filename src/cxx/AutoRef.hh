/*
** Copyright (C) 2012 Sylvain Fargier <fargier.sylvain@free.fr>
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
** autoRef.hh
**
**  Created on: Oct 27, 2010
**      Author: sfargier
*/

#ifndef __AUTOREF_HH__
#define __AUTOREF_HH__

#include <cstddef>

template <class C> class AutoRef;

template <class C>
bool operator < (const AutoRef< C > &c1, const AutoRef< C > &c2);


/**
 * @brief Convenience auto-refcount class
 *
 * @details Increments the object's refcount upon creation and decrements it when released.
 * The object must implement incRef/decRef methods (provided by CRefCount class
 */
template <class C>
class AutoRef {
public:
    AutoRef(C *c = NULL) throw () :
        m_class(NULL)
    {
        acquire(c);
    }

    AutoRef(const AutoRef &r) throw () :
        m_class(NULL)
    {
        acquire(r.m_class);
    }

    ~AutoRef()
    {
        release();
    }

    AutoRef<C> &operator = (const AutoRef<C> &r) throw ()
    {
        if (this != &r)
            acquire(r.m_class);
        return *this;
    }

    AutoRef<C> &operator = (C *p) throw () {
        acquire(p);
        return *this;
    }

    C &operator * () const throw() { return *m_class; };
    C *operator -> () const throw() { return m_class; };
    C *get() const throw() { return m_class; };
    bool operator == (const AutoRef<C> &a) const { return m_class == a.m_class; };
    bool operator != (const AutoRef<C> &a) const { return m_class != a.m_class; };

protected:
    C *m_class;

    inline void release() throw()
    {
        if (m_class) {
            m_class->decRef();
            m_class = NULL;
        }
    }

    inline void acquire(C *c) throw()
    {
        if (m_class)
            m_class->decRef();
        m_class = c;
        if (m_class)
            c->incRef();
    }

    friend bool operator == (const C *c, const AutoRef< C >&a)
            { return c == a.m_class; }
    friend bool operator != (const C *c, const AutoRef< C >&a)
            { return c != a.m_class; }
    friend bool operator == (const AutoRef< C >&a, const C *c)
            { return c == a.m_class; }
    friend bool operator != (const AutoRef< C >&a, const C *c)
            { return c != a.m_class; }

    friend bool operator< <C>(const AutoRef< C > &, const AutoRef< C > &);
};

template <class C>
bool operator < (const AutoRef< C > &c1, const AutoRef< C > &c2) {
    if (!c1.m_class || !c2.m_class)
        return c1.m_class < c2.m_class;
    else
        return *c1 < *c2;

}

#endif /* __AUTOREF_HH__ */
