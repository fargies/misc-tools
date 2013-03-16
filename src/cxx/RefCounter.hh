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
** refCounter.hh
**
**        Created on: May 05, 2011
**            Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef REFCOUNTER_HH_
#define REFCOUNTER_HH_

/**
 * @brief Implementation of RefCounter
 */
class RefCounter
{
public:
    int incRef();

    /**
     * @brief decrement refCount
     *
     * @details the del parameter is mostly used with RefCounted attributes in a class
     * @param del whether the object should be deleted when the refCount reaches 0
     * @return current refCount
     */
    int decRef(bool del = true);
    int getNumRef() const; //< Test/Debug purpose

protected:
    int m_numRef;

    RefCounter();
    virtual ~RefCounter();
};

#endif /*REFCOUNTER_HH_*/
