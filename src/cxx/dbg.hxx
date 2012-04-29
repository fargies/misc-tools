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
** dbg.hxx
**
**        Created on: Apr 26, 2012
**   Original Author: fargie_s
**
*/

#ifndef __DBG_HXX__
#define __DBG_HXX__

#if !defined(NDEBUG)

template <typename T>
Debug &Debug::operator <<(const HexFmt<T> &f)
{
    size_t buff_size = DEBUG_BUFF_SIZE - m_pos;

    if (m_pos && (sizeof(T) * 2 + 2) > buff_size)
        flush();

    m_pos += snprintf(&buf()[m_pos], buff_size, "0x%.*X",
            (int) sizeof (T) * 2, f.m_val);
    return *this;
}

#endif

template <typename T>
inline Debug::HexFmt<T> Hex(const T &t)
{
    return Debug::HexFmt<T>(t);
}

#endif

