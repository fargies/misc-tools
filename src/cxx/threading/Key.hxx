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
** Key.hxx
**
**        Created on: Nov 25, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __KEY_HXX__
#define __KEY_HXX__

#include "Key.hh"

namespace threading {

template <class C>
Key<C>::Key(C *c)
{
    pthread_key_create(&m_key, Key::destroy);
    if (c)
        pthread_setspecific(m_key, c);
}

template <class C>
Key<C>::~Key()
{
    C *ref = static_cast<C *>(pthread_getspecific(m_key));

    if (ref)
        delete ref;
    /* let's hope other threads have cleaned up their data */
    pthread_key_delete(m_key);
}

template <class C>
C &Key<C>::operator * ()
{
    C *ref = static_cast<C *>(pthread_getspecific(m_key));

    if (ref == NULL)
    {
        //log_critical("[Key]: accessing uninitialized pointer.");
    }
    return *ref;
}

template <class C>
const C &Key<C>::operator * () const
{
    C *ref = static_cast<C *>(pthread_getspecific(m_key));

    if (ref == NULL)
    {
        //log_critical("[Key]: accessing uninitialized pointer.");
    }
    return *ref;
}

template <class C>
C *Key<C>::operator -> ()
{
    return static_cast<C *>(pthread_getspecific(m_key));
}

template <class C>
const C *Key<C>::operator -> () const
{
    return static_cast<C *>(pthread_getspecific(m_key));
}

template <class C>
Key<C> &Key<C>::operator = (C *val)
{
    C *ref = static_cast<C *>(pthread_getspecific(m_key));

    if (ref && (ref != val))
        delete ref;

    pthread_setspecific(m_key, val);
    return *this;
}

template <class C>
void Key<C>::destroy(void * ptr)
{
    C *ref = static_cast<C *>(ptr);

    if (ref)
        delete ref;
}

}

#endif

