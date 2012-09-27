/*
** Copyright (C) 2012 Sylvain Fargier <sylvain.fargier@somfy.com>
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
** AutoPtr.hh
**
**        Created on: May 09, 2011
**   Original Author: Sylvain Fargier <sylvain.fargier@somfy.com>
**
*/

#ifndef __AUTOPTR_HH__
#define __AUTOPTR_HH__

template <class C>
class AutoPtr
{
public:
    AutoPtr(C *c = NULL) :
        m_ptr(c)
    {}

    AutoPtr(const AutoPtr &p) :
        m_ptr(NULL)
    {
        this->operator =(p);
    }

    AutoPtr<C> &operator =(const AutoPtr<C> &p)
    {
        if (m_ptr != p.m_ptr) {
            if (m_ptr)
                delete m_ptr;

            m_ptr = p.m_ptr;
            const_cast<AutoPtr<C>&> (p).m_ptr = NULL;
        }
        return *this;
    }

    ~AutoPtr()
    {
        if (m_ptr)
            delete m_ptr;
    }

    C& operator *() const
    { return *m_ptr; };
    C* operator ->() const
    { return m_ptr; };
    C* get() const
    { return m_ptr; };

    friend bool operator == (const C* c, const AutoPtr<C>& a)
    { return c == a.m_ptr; }
    friend bool operator != (const C* c, const AutoPtr<C>& a)
    { return c != a.m_ptr; }
    friend bool operator == (const AutoPtr<C>& a, const C* c)
    { return c == a.m_ptr; }
    friend bool operator != (const AutoPtr<C>& a, const C* c)
    { return c != a.m_ptr; }

protected:
    C *m_ptr;
};

#endif

