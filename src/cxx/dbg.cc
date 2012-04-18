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
** dbg.cc
**
**        Created on: Apr 14, 2012
**   Original Author: fargie_s
**
*/

#if !defined(NDEBUG)

#include <cstdio>
#include <cstring>

#include "dbg.hh"

#define DEBUG_BUFF_SIZE 4096

#define DEBUG_TRUE_STR  "true"
#define DEBUG_FALSE_STR "false"
#define DEBUG_NULL_STR  "null"
#define DEBUG_INDENT_STR "  "
#define DEBUG_INDENT_LEN (sizeof (DEBUG_INDENT_STR) - 1)
#define DEBUG_MAX_INDENT (DEBUG_BUFF_SIZE / DEBUG_INDENT_LEN / 2)

static char buffer[DEBUG_BUFF_SIZE];


Debug &Debug::operator <<(Debug::DebugMod m)
{
    switch (m)
    {
        case NL:
            //put('\n');
            flush();
            break;
        case INDENT:
            ++m_indent;
            break;
        case OUTDENT:
            --m_indent;
            break;
    };
    return *this;
}

Debug &Debug::operator <<(const char *str)
{
    if (str == NULL)
        put(DEBUG_NULL_STR);
    else
        put(str);
    return *this;
}

Debug &Debug::operator <<(const Quoted &f)
{
    if (!f.m_str)
        put(DEBUG_NULL_STR);
    else
    {
        const char *str = f.m_str;
        if (m_pos && (m_pos + strlen(str) + 2) > DEBUG_BUFF_SIZE)
            flush();

        buffer[m_pos++] = '"';
        while (*str != '\0' && m_pos < DEBUG_BUFF_SIZE)
            buffer[m_pos++] = *(str++);
        if (m_pos < DEBUG_BUFF_SIZE)
            buffer[m_pos++] = '"';
    }
    return *this;
}

Debug &Debug::operator <<(int i)
{
    size_t buff_size = DEBUG_BUFF_SIZE - m_pos;
    size_t len = snprintf(&buffer[m_pos], buff_size, "%d", i);

    if (len >= buff_size)
    {
        flush();
        buff_size = DEBUG_BUFF_SIZE - m_pos;
        len = snprintf(&buffer[m_pos], buff_size, "%d", i);
        m_pos += (len > buff_size) ? buff_size : len;
    }
    else
        m_pos += len;
    return *this;
}

Debug &Debug::operator <<(unsigned int i)
{
    size_t buff_size = DEBUG_BUFF_SIZE - m_pos;
    size_t len = snprintf(&buffer[m_pos], buff_size, "%u", i);

    if (len >= buff_size)
    {
        flush();
        buff_size = DEBUG_BUFF_SIZE - m_pos;
        len = snprintf(&buffer[m_pos], buff_size, "%u", i);
        m_pos += (len > buff_size) ? buff_size : len;
    }
    else
        m_pos += len;
    return *this;
}

Debug &Debug::operator <<(char c)
{
    if (m_pos && (m_pos + 3) > DEBUG_BUFF_SIZE)
        flush();

    if ((m_pos + 3) <= DEBUG_BUFF_SIZE)
    {
        buffer[m_pos++] = '\'';
        buffer[m_pos++] = c;
        buffer[m_pos++] = '\'';
    }
    return *this;
}

Debug &Debug::operator <<(bool b)
{
    (b) ? put(DEBUG_TRUE_STR) : put(DEBUG_FALSE_STR);
    return *this;
}

Debug &Debug::operator <<(short s)
{
    this->operator <<(static_cast<unsigned int>(s));
    return *this;
}

void Debug::put(const char *str)
{
    if (m_pos && (m_pos + strlen(str)) > DEBUG_BUFF_SIZE)
        flush();

    while (*str != '\0' && m_pos < DEBUG_BUFF_SIZE)
        buffer[m_pos++] = *(str++);
}

void Debug::put(char c)
{
    if (m_pos == DEBUG_BUFF_SIZE)
        flush();

    buffer[m_pos++] = c;
}

void Debug::flush()
{
    if (m_pos)
    {
        fwrite(buffer, m_pos, 1, stderr);
        fputc('\n', stderr);
        fflush(stderr);
        m_pos = 0;

        unsigned int indent = (m_indent > DEBUG_MAX_INDENT) ?
                DEBUG_MAX_INDENT : m_indent;
        char *tmp_buf = buffer;
        while (indent--)
        {
            memcpy(tmp_buf, DEBUG_INDENT_STR, DEBUG_INDENT_LEN);
            tmp_buf += DEBUG_INDENT_LEN;
            m_pos += DEBUG_INDENT_LEN;
        }
    }
}

#ifdef DEBUG_STL
Debug &operator <<(Debug &d, const std::string &s)
{
    d << Debug::Quoted(s.c_str());
    return d;
}

#endif

#endif

