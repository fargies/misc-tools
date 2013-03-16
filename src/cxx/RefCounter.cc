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
** refcounter.cc
**
**        Created on: Nov 07, 2012
**   Original Author: Sylvain Fargier <fargier.sylvain@free.fr>
**
*/

#include "RefCounter.hh"
#include <assert.h>

int RefCounter::incRef()
{
    return ++m_numRef;
}

int RefCounter::decRef(bool del)
{
    int nr = --m_numRef;
    assert((m_numRef >= 0) && "RefCounter : too much decRef");
    if (m_numRef < 1 && del)
        delete this;
    return nr;

}

int RefCounter::getNumRef() const
{
    return m_numRef;
}

RefCounter::RefCounter() :
    m_numRef(0)
{
}

RefCounter::~RefCounter()
{
    assert((m_numRef == 0) && "RefCounter : don't manual delete !");
}

