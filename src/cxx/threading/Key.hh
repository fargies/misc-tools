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
** Key.hh
**
**        Created on: Nov 25, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __KEY_HH__
#define __KEY_HH__

#include <pthread.h>

#include "AutoRef.hh"
#include "RefCounter.hh"

namespace threading {

/**
 * @brief pthread key class.
 *
 * @details any thread specific data not in current thread must be manually
 * released before destroying this object, only the data allocated by the
 * releasing thread will be automatically released.
 *
 * AutoKey objects might be used to automatically release thread specific data.
 */
template <class C>
class Key : public RefCounter
{
public:
    Key(C *c = NULL);

    ~Key();

    C &operator * ();
    const C &operator * () const;

    C *operator -> ();
    const C *operator -> () const;

    Key &operator = (C *val);

    bool operator == (const C* c) const
    { return c == this->operator ->(); }
    bool operator != (const C* c) const
    { return c != this->operator ->(); }

    friend bool operator == (const C *c, const Key<C> &k)
    { return k == c; }
    friend bool operator != (const C* c, const Key<C>& k)
    { return k != c; }

protected:
    pthread_key_t m_key;

private:
    static void destroy(void * ptr);
};

template <class C>
class AutoKey : public AutoRef<C>
{
public:
    AutoKey(Key<C> &k) :
        AutoRef<C>(k)
    {}

    ~AutoKey()
    {
        /* release any thread specific data */
        if (AutoRef<C>::m_class)
            *AutoRef<C>::m_class = NULL;
    }
};

}

#include "Key.hxx"

#endif

