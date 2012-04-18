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
** dbg.hh
**
**        Created on: Apr 14, 2012
**   Original Author: fargie_s
**
*/

#ifndef __DBG_HH__
#define __DBG_HH__

#define DEBUG_STL

struct NoFormat
{
    NoFormat(const char *str) :
        m_str(str)
    {}
    const char *m_str;
};

class Debug
{
public:
    Debug() :
        m_pos(0), m_indent(0)
    {
    }

    ~Debug()
    {
        *this << NL;
    }

    inline Debug &ref()
    {
        return *this;
    }

    enum DebugMod
    {
        NL,
        INDENT,
        OUTDENT
    };

#if defined(NDEBUG)
    template <typename T>
        Debug &operator <<(T);

#else

    Debug &operator <<(bool b);
    Debug &operator <<(char);
    Debug &operator <<(short);
    Debug &operator <<(unsigned short);
    Debug &operator <<(int);
    Debug &operator <<(unsigned int);
    Debug &operator <<(const char *);
    Debug &operator <<(DebugMod);
    Debug &operator <<(const NoFormat &);

#endif

protected:
    void puts(const char *);
    void putc(char);
    void flush();

private:
    unsigned int m_pos;
    unsigned int m_indent;
};

#if defined(NDEBUG)
#define DBG if (0) Debug()
#define DBG(s) if (0) Debug()
#else
#define DBG Debug().ref()
#define SDBG(s) Debug().ref() << NoFormat(s)
#endif

#ifdef DEBUG_STL

#include <string>
#include <list>
#include <map>

template <typename T>
Debug &operator <<(Debug &d, const std::list<T> &l)
{
    d << "[ " << Debug::INDENT << Debug::NL;

    bool first = true;
    for (typename std::list<T>::const_iterator it = l.begin();
            it != l.end(); ++it)
    {
        if (first)
        {
            d << *it;
            first = false;
        }
        else
            d << ", " << Debug::NL << *it;
    }
    d << " ]" << Debug::OUTDENT << Debug::NL;
    return d;
}

template <typename K, typename V>
Debug &operator <<(Debug &d, const std::map<K, V> &m)
{
    d << NoFormat("{ ") << Debug::INDENT << Debug::NL;

    bool first = true;
    for (typename std::map<K, V>::const_iterator it = m.begin();
            it != m.end(); ++it)
    {
        if (first)
        {
            d << it->first << NoFormat(" : ") << it->second;
            first = false;
        }
        else
            d << NoFormat(", ") << Debug::NL <<
                it->first << NoFormat(" : ") << it->second;
    }
    d << NoFormat(" }") << Debug::OUTDENT << Debug::NL;
}

Debug &operator <<(Debug &d, const std::string &);

#endif

#endif

