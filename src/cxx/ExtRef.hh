/*
** Copyright (C) 2012 Fargier Sylvain <fargier.sylvain@free.fr>
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
** extRef.hh
**
**        Created on: Nov 09, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __EXT_REF_HH__
#define __EXT_REF_HH__

/**
 * @brief External refcounter.
 *
 * @details the given object doesn't need to be refcounted, it will be
 * externally.
 */
template <class C>
class ExtRef
{
public:
    ExtRef(C *ptr) :
        m_ptr(ptr),
        m_cnt(new int(1))
    {}

    ExtRef(const ExtRef &ref) :
        m_ptr(ref.m_ptr),
        m_cnt(ref.m_cnt)
    {
        incRef();
    }

    ExtRef &operator =(const ExtRef &ref)
    {
        if (this != &ref)
        {
            const_cast<ExtRef &>(ref).incRef();
            decRef();
            m_cnt = ref.m_cnt;
            m_ptr = ref.m_ptr;
        }
        return *this;
    }

    ~ExtRef()
    {
        decRef();
    }

    void decRef()
    {
        if (--(*m_cnt) == 0)
        {
            delete m_cnt;
            if (m_ptr)
                delete m_ptr;
        }
    }

    void incRef()
    {
        ++(*m_cnt);
    }

    int getNumRef() const
    {
        return *m_cnt;
    }

    C& operator *() const
    { return *m_ptr; };
    C* operator ->() const
    { return m_ptr; };
    C* get() const
    { return m_ptr; };

    friend bool operator == (const C* c, const ExtRef<C>& a)
    { return c == a.m_ptr; }
    friend bool operator != (const C* c, const ExtRef<C>& a)
    { return c != a.m_ptr; }
    friend bool operator == (const ExtRef<C>& a, const C* c)
    { return c == a.m_ptr; }
    friend bool operator != (const ExtRef<C>& a, const C* c)
    { return c != a.m_ptr; }

private:
    C *m_ptr;
    int *m_cnt;
};


#endif

